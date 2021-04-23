#include "process.h"
#include "linux_parser.h"

#include <unistd.h>

#include <cctype>
#include <cmath>
#include <ctime>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

namespace lp = LinuxParser;

Process::Process(int pid) : pid_(pid) {
	int uid = lp::Uid(pid);
	vector<long> data;
	this->command_ = lp::Command(pid);
	this->user_ = lp::User(uid);
	this->start_time_ = lp::StartTime(pid);
	this->cpu_data_.push_back(data);
	this->cpu_data_.push_back(data);
}

int Process::Pid() { return this->pid_; }

float Process::CpuUtilization() {
	std::time_t now = std::time(nullptr);
	float cpu_util{0.0f};

	if ((now - this->data_updated_ > kDataUpdateInterval) ||
		(this->cpu_data_.size() == 0))
	{
		this->UpdateData();
	}

	cpu_util = 1000 * ((float)this->cpu_data_[kJiffiesProc_][lp::kPresent_] - 
			(float)this->cpu_data_[kJiffiesProc_][lp::kPast_]) /
			((float)this->cpu_data_[kJiffiesSys_][lp::kPresent_] -
			(float)this->cpu_data_[kJiffiesSys_][lp::kPast_]);

	if (std::isnan(cpu_util))
	{
		cpu_util = 0.0f;
	}
	this->cpu_util_ = cpu_util;
	return cpu_util;
}

string Process::Command() { return this->command_; }

string Process::Ram() { return lp::Ram(this->pid_); }

string Process::User() { return this->user_; }

long Process::UpTime() {
	long now;
	long uptime{0l};

	now = lp::UpTime();
	uptime = now - this->start_time_;
	return uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a [[maybe_unused]]) const {
    return true;
}

void Process::UpdateData() {
	std::vector<long> jiffies_proc;
	long jiffies_proc_total{0l};
	long jiffies_system{0l};
	std::time_t now;
	
	jiffies_system = lp::TotalJiffies();
	this->AddValue(kJiffiesSys_, jiffies_system);
	jiffies_proc = lp::PidStat(this->pid_);
	jiffies_proc_total = std::accumulate(jiffies_proc.begin(), jiffies_proc.end() - 1, 0);
	this->AddValue(kJiffiesProc_, jiffies_proc_total);

	now = std::time(nullptr);
	this->data_updated_ = now;
}

void Process::AddValue(int jiffy_type, long value) {
	if (this->cpu_data_[jiffy_type].size() != 2)
	{
		cpu_data_[jiffy_type].push_back(value);
		cpu_data_[jiffy_type].push_back(value);
	}
	else
	{
		this->cpu_data_[jiffy_type].erase(this->cpu_data_[jiffy_type].begin());
		this->cpu_data_[jiffy_type].push_back(value);
	}
}