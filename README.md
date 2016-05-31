# tsxmacros-stats
Usefull macros for quick defining of transactional regions and stats gathering

Flags explanation:
INCLUDE_RTMLIB: Include your own rtm library. In some systems you dont have to cause its in the compiler's path
RTM_GATHER_STATS: Gather stats for your transactions.

Statistics gathering example:
  1) Get your global unique ReportStat instance 
      ReportStat& stats = ReportStat::getInstance();
  2) Print them (Change layout in the overloaded << operator in the ReportStat.h file)
      std::cout << stats << std::endl;
