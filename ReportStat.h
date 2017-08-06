#include <mutex>
#include <sched.h>
#include <stdio.h>

class ReportStat{
	public:
		static ReportStat& getInstance(){
			static ReportStat instance;
			return instance;
		}
	private:
		ReportStat(){
			max_threads = 88;
			initialise();
		};

		void initialise(){
			stats = (per_thread_stats_t **) malloc(max_threads*sizeof(per_thread_stats_t *));
			for (int i =0; i < max_threads; i++){
				stats[i] = (per_thread_stats_t *)malloc(sizeof(per_thread_stats_t));
				stats[i]->abort_conflict = 0;
				stats[i]->abort_capacity = 0;
				stats[i]->abort_retry = 0;
				stats[i]->abort_explicit = 0;
				stats[i]->abort_debug = 0;
				stats[i]->abort_nested = 0;
				stats[i]->aborts = 0;
				stats[i]->transactions_completed = 0;
				stats[i]->locks = 0;
			}

		}
		int max_threads;

		typedef struct per_thread_stats{
			int abort_conflict, abort_capacity, abort_retry, abort_explicit, abort_debug, abort_nested, aborts, transactions_completed, locks;
			int dummy[7];
		}per_thread_stats_t;
		per_thread_stats_t ** stats;

	public:
		ReportStat(ReportStat const&) = delete;
		void operator=(ReportStat const&) = delete;

		void report_status(int status){
			int thread_id = sched_getcpu();

			stats[thread_id]->aborts++;
			if(status & _XABORT_CONFLICT){
				stats[thread_id]->abort_conflict += 1;
			}else if (status & _XABORT_CAPACITY){
				stats[thread_id]->abort_capacity += 1;
			}else if (status & _XABORT_RETRY){
				stats[thread_id]->abort_retry += 1;
			}else if (status & _XABORT_EXPLICIT){
				stats[thread_id]->abort_explicit += 1;
			}else if (status & _XABORT_DEBUG){
				stats[thread_id]->abort_debug += 1;
			}else if (status & _XABORT_NESTED){
				stats[thread_id]->abort_nested += 1;
			}
		}

		void report_transaction(){
			int thread_id = sched_getcpu();

			stats[thread_id]->transactions_completed += 1;
		}

		void report_lock(){
			int thread_id = sched_getcpu();

			stats[thread_id]->locks += 1;
		}

		int getTotalAborts() const {
			int total_aborts = 0;
			for (int i=0; i < max_threads; i++){
				total_aborts += stats[i]->aborts;
			}
			return total_aborts;
		}

		int getConflictAborts() const {
			int total_aborts_conflict = 0;
			for (int i=0; i < max_threads; i++){
				total_aborts_conflict += stats[i]->abort_conflict;
			}
			return total_aborts_conflict;
		}

		int getCapacityAborts() const{
			int total_aborts_capacity = 0;
			for (int i=0; i < max_threads; i++){
				total_aborts_capacity += stats[i]->abort_capacity;
			}
			return total_aborts_capacity;
		}

		int getRetryAborts() const{
			int total_aborts_retry = 0;
			for(int i=0; i < max_threads; i++){
				total_aborts_retry += stats[i]->abort_retry;
			}
			return total_aborts_retry;
		}

		int getExplicitAborts() const {
			int total_aborts_explicit = 0;
			for (int i=0; i < max_threads; i++){
				total_aborts_explicit += stats[i]->abort_explicit;
			}
			return total_aborts_explicit;
		}

		int getDebugAborts() const {
			int total_aborts_debug = 0;
			for (int i=0; i < max_threads; i++){
				total_aborts_debug += stats[i]->abort_debug;
			}
			return total_aborts_debug;
		}

		int getNestedAborts() const {
			int total_aborts_nested = 0;
			for(int i=0; i < max_threads; i++){
				total_aborts_nested += stats[i]->abort_nested;
			}
			return total_aborts_nested;
		}

		int getTotalCompletedTransactions() const{
			int total_transactions_completed = 0;
			for (int i=0; i < max_threads; i++){
				total_transactions_completed += stats[i]->transactions_completed;
			}
			return total_transactions_completed;
		}

		int getTotalLocks() const {
			int total_locks = 0;
			for(int i=0; i < max_threads ; i++){
				total_locks += stats[i]->locks;
			}
			return total_locks;
		}

		friend std::ostream& operator<<(std::ostream& stream, const ReportStat &RS){
			std::cout << "\tTotal transactions completed: " << RS.getTotalCompletedTransactions() << std::endl;
			std::cout << "\tConflict aborts: " << RS.getConflictAborts() << std::endl;
			std::cout << "\tCapacity aborts: " << RS.getCapacityAborts() << std::endl;
			std::cout << "\tRetry aborts: " << RS.getRetryAborts() << std::endl;
			std::cout << "\tExplicit aborts: " << RS.getExplicitAborts() << std::endl;
			std::cout << "\tDebug aborts: " << RS.getDebugAborts() << std::endl;
			std::cout << "\tNested aborts: " << RS.getNestedAborts() << std::endl;
			std::cout << "\tTotal aborts: " << RS.getTotalAborts() << std::endl;
			std::cout << "\tTotal locks: " << RS.getTotalLocks() << std::endl;
			return stream;
		}
};


