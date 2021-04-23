#ifndef PROCESS_H
#define PROCESS_H

#include <ctime>
#include <string>
#include <vector>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/

// Interval for process CPU usage caching.
const int kDataUpdateInterval = 1;

class Process {
   public:
   	Process(int pid);
    int Pid();
    std::string User();                  
    std::string Command();       
    float CpuUtilization();
    std::string Ram();             
    long UpTime();    
    bool operator<(Process const &a) const;  // TODO: See src/process.cpp
    void UpdateData();
    void UpdateResult();

   private:
    int pid_{0};
    std::string user_ = "";
    std::string command_ = "";
    long start_time_{0};

    void AddValue(int jiffy_type, long value);

    std::vector<std::vector<long>> cpu_data_ = {};
    float cpu_util_{0};
    std::time_t data_updated_{0};
};

enum JiffyData {
	kJiffiesSys_ = 0,
	kJiffiesProc_
};

#endif