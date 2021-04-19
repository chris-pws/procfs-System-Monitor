#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

namespace lp = LinuxParser;

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor &System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process> &System::Processes() { return processes_; }

std::string System::Kernel() { return lp::Kernel(); }

float System::MemoryUtilization() { return lp::MemoryUtilization(); }

std::string System::OperatingSystem() { return lp::OperatingSystem(); }

int System::RunningProcesses() { return lp::RunningProcesses(); }

int System::TotalProcesses() { return lp::TotalProcesses(); }

long int System::UpTime() { return lp::UpTime(); }