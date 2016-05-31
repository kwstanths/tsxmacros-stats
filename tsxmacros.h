#define __try_transaction(x) if ((x = _xbegin()) == _XBEGIN_STARTED)
#define __transaction_abort(c) _xabort(c);
#define __transaction_end if (_xtest()) _xend();

#include "ReportStat.h"
#include <pthread.h>

static inline int transaction_start(pthread_spinlock_t * fallback_lock, int number_of_tries){

#ifdef RTM_GATHER_STATS
	 ReportStat& stats = ReportStat::getInstance();
#endif

	int status;
	int aborts=0;
	while(1){
		while(*fallback_lock == 0);

		__try_transaction(status){
			if (*fallback_lock == 0) __transaction_abort(0xff);		
			return number_of_tries - aborts;	
		}

#ifdef RTM_GATHER_STATS
		stats.report_status(status);
#endif
		if (aborts++ == number_of_tries){
			pthread_spin_lock(fallback_lock);
			return number_of_tries - aborts;
		}
	}
	return -1;
}

static inline int transaction_end(pthread_spinlock_t * fallback_lock){
	
#ifdef RTM_GATHER_STATS
	ReportStat& stats = ReportStat::getInstance();
#endif
	if (*fallback_lock == 1){
		__transaction_end;
#ifdef RTM_GATHER_STATS
		stats.report_transaction();
#endif
		return 0;
	}else{
		pthread_spin_unlock(fallback_lock);
		return 1;
	}
	
}

