#ifdef INCLUDE_RTMLIB
#include "tsx-tools-master/include/rtm.h"
#endif

#include <mutex>
#include <iostream>

class ReportStat{
	public:
		static ReportStat& getInstance(){
			static ReportStat instance;
			return instance;
		}
	private:
		ReportStat(){
			abort_conflict=0;
			abort_capacity=0;
			abort_retry=0;
			abort_explicit=0;
			abort_debug=0;
			abort_nested=0;
			total_aborts=0;
			total_transactions_completed=0;
		};
		int abort_conflict, abort_capacity, abort_retry, abort_explicit, abort_debug, abort_nested, total_aborts, total_transactions_completed;
		std::mutex lock;
		std::mutex lock2;

	public:
		ReportStat(ReportStat const&) = delete;
		void operator=(ReportStat const&) = delete;

		void report_status(int status){
			lock.lock();
			total_aborts++;
			if(status & _XABORT_CONFLICT){
				abort_conflict++;
			}else if (status & _XABORT_CAPACITY){
				abort_capacity++;
			}else if (status & _XABORT_RETRY){
				abort_retry++;
			}else if (status & _XABORT_EXPLICIT){
				abort_explicit++;
			}else if (status & _XABORT_DEBUG){
				abort_debug++;
			}else if (status & _XABORT_NESTED){
				abort_nested++;
			}
			lock.unlock();
		}

		void report_transaction(){
			lock2.lock();
			total_transactions_completed++;
			lock2.unlock();
		}

		int getConflictAborts(){
			return abort_conflict;
		}
		int getCapacityAborts(){
			return abort_capacity;
		}
		int getRetryAborts(){
			return abort_retry;
		}
		int getExplicitAborts(){
			return abort_explicit;
		}
		int getDebugAborts(){
			return abort_debug;
		}
		int getNestedAborts(){
			return abort_nested;
		}
		int getTotalAborts(){
			return total_aborts;
		}
		int getTotalCompletedTransactions(){
			return total_transactions_completed;
		}

		friend std::ostream& operator<<(std::ostream& stream, const ReportStat &RS){
			std::cout << "\tTotal transactions completed: " << RS.total_transactions_completed << std::endl;
			std::cout << "\tConflict aborts: " << RS.abort_conflict << std::endl;
			std::cout << "\tCapacity aborts: " << RS.abort_capacity << std::endl;
			std::cout << "\tRetry aborts: " << RS.abort_retry << std::endl;
			std::cout << "\tExplicit aborts: " << RS.abort_explicit << std::endl;
			std::cout << "\tDebug aborts: " << RS.abort_debug << std::endl;
			std::cout << "\tNested aborts: " << RS.abort_nested << std::endl;
			std::cout << "\tTotal aborts: " << RS.total_aborts << std::endl;

			return stream;
		}
};
