#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>

namespace LinuxParser {

const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

enum CPUState {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
};

enum CPUData { 
    kPast_ = 0, 
    kPresent_ 
};

enum MemInfo {
    kMemTotal_ = 0,
    kMemFree_,
};

enum PidStat { 
    kUTime_ = 0, 
    kSTime_, 
    kCUTime_, 
    kCSTime_, 
    kStartTime_ 
};

std::string Command(int pid);
std::vector<long> PidStat(int pid);
std::string Ram(int pid);
long TotalJiffies();
int Uid(int pid);
std::string User(int pid);
long StartTime(int pid);

std::string NthToken(std::string line, int token_pos);
int SysClk();
};  // namespace LinuxParser

#endif