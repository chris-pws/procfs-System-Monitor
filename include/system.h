#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

/********* System class *******
 *  Unifier that provides interfacing to the system monitor modules.
 */
class System {
   public:
    Processor& Cpu();
    std::vector<Process>& Processes();
    float MemoryUtilization();
    long UpTime();
    int TotalProcesses();
    int RunningProcesses();
    std::string Kernel();
    std::string OperatingSystem();

   private:
    Processor cpu_ = {};
    std::vector<Process> processes_ = {};

    std::vector<int> DumpPids();
    std::vector<int> NoPartner(const std::vector<int> partner_a,
                               const std::vector<int> partner_b);
    void AddProcs(const std::vector<int> pid_list);
    void PruneProcs(const std::vector<int> dead_pids);
    void SortProcsByCpu();
};

#endif