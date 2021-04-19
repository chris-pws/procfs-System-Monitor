#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cmath>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
    string line;
    string key;
    string value = "";
    std::ifstream fs(kOSPath);
    if (fs.is_open()) {
        while (std::getline(fs, line)) {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "PRETTY_NAME") {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    fs.close();
                    return value;
                }
            }
        }
    }
    fs.close();
    return value;
}

string LinuxParser::Kernel() {
    string os, kernel, version;
    string line;
    std::ifstream fs(kProcDirectory + kVersionFilename);
    if (fs.is_open()) {
        std::getline(fs, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    fs.close();
    return kernel;
}

vector<int> LinuxParser::Pids() {
    vector<int> pids;
    string stem;
    for (auto& p : std::filesystem::directory_iterator(kProcDirectory)) {
        stem = p.path().stem();
        stem.erase(remove(stem.begin(), stem.end(), '\"'), stem.end());
        if (std::all_of(stem.begin(), stem.end(), isdigit)) {
            pids.push_back(stoi(stem));
        }
    }
    return pids;
}

float LinuxParser::MemoryUtilization() {
    string line;
    int i{0};
    string line_split;

    float v{0.0f};
    vector<float> mem_values;
    float mem_used{0.0f};
    float mem_usage_pct = 0.0f;

    std::ifstream fs(kProcDirectory + kMeminfoFilename);
    if (fs.is_open()) {
        while ((std::getline(fs, line)) && i < 2) {
            auto pos = line.find(":");
            line_split = line.substr(pos + 1, -1);

            std::istringstream ss(line_split);
            ss >> v;
            mem_values.push_back(v);
            i++;
        }
    }

    fs.close();

    mem_used = (mem_values[kMemTotal_] - mem_values[kMemFree_]);
    mem_usage_pct = mem_used / mem_values[kMemTotal_];

    if (std::isnan(mem_usage_pct)) {
        return 0.0f;
    } else {
        return mem_usage_pct;
    }
}

long LinuxParser::UpTime() {
    string line;
    long result{0};

    std::ifstream fs(kProcDirectory + kUptimeFilename);
    if (fs.is_open()) {
        std::getline(fs, line);
        std::istringstream ss(line);
        ss >> result;
    }

    return result;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

int LinuxParser::TotalProcesses() {
    int total_processes{0};
    string stem;

    for (auto& p : std::filesystem::directory_iterator(kProcDirectory)) {
        stem = p.path().stem();
        if (std::all_of(stem.begin(), stem.end(), isdigit)) {
            total_processes++;
        }
    }
    return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line;
    string key = "procs_running";
    int running_processes{0};

    std::ifstream fs(kProcDirectory + kStatFilename);
    while(std::getline(fs, line)) {
        if (line.find(key) != std::string::npos) {
            std::stringstream ss(line);
            ss >> key >> running_processes; 
        }
    }
    return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
int LinuxParser::Uid(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
