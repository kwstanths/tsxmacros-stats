# tsxmacros-stats
Usefull macros for quick defining of transactional regions and stats gathering



a)  Flags explanation:

    INCLUDE_RTMLIB: Include your own rtm library. In some systems you dont have to cause its in the compiler's path
    RTM_GATHER_STATS: Gather stats for your transactions.



b)  Transaction area example:

    transaction_start(&your_own_fallback_lock, number_of_transaction_tries);
      //Transactional area
    transaction_end(&your_own_fallback_lock);
    //fallback_lock is very important as the transactions may abort repeatedly and the program whould seem to never 
    end


c)  Statistics gathering example:

    1) Get your global unique ReportStat instance 
        ReportStat& stats = ReportStat::getInstance();
    2) Print them (Change layout in the overloaded << operator in the ReportStat.h file)
        std::cout << stats << std::endl;
