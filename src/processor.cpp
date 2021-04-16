#include "processor.h"

#include <sys/time.h>

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

namespace lp = LinuxParser;

std::vector<float> Processor::Utilization() {
    std::time_t now = std::time(nullptr);

    if ((now - this->data_updated_ > DATA_DELTA) || (this->cpu_data_.empty())) {
        UpdateData();
    }
    if ((now - this->result_updated_ > RESULT_DELTA) ||
        (this->cpu_result_.empty())) {
        UpdateResult();
    }

    return cpu_result_;
}

void Processor::UpdateData() {
    std::string line = "";
    std::string line_elements = "";
    int state_value{0};
    int count{0};
    std::vector<int> cpu_state;
    int idle{0};
    int non_idle{0};
    std::time_t now;

    std::ifstream fs(lp::kProcDirectory + lp::kStatFilename);
    if (fs.is_open()) {
        // Skip the first line (CPUn average).
        std::getline(fs, line);

        while (std::getline(fs, line)) {
            if (line.find("cpu") == std::string::npos) {
                break;
            }
            line_elements = line.substr(5, line.back());

            std::stringstream ss(line_elements);
            while (ss >> state_value) {
                cpu_state.push_back(state_value);
            }

            idle = cpu_state[lp::kIdle_] + cpu_state[lp::kIOwait_];
            non_idle = cpu_state[lp::kUser_] + cpu_state[lp::kNice_] +
                       cpu_state[lp::kSystem_] + cpu_state[lp::kIRQ_] +
                       cpu_state[lp::kSoftIRQ_] + cpu_state[lp::kSteal_];

            this->AddCpuSample(count, idle, non_idle);
            cpu_state.clear();
            count++;
        }

        fs.close();
    }

    now = std::time(nullptr);
    this->data_updated_ = now;
}

void Processor::UpdateResult() {
    float total{0.0f};
    float total_prev{0.0f};
    float totald{0.0f};
    float idled{0.0f};
    float cpu_usage_pct{0.0f};
    std::vector<float> result;
    std::time_t now;

    for (std::vector<CpuNData> cpu : this->cpu_data_) {
        total = cpu[1].idle + cpu[1].non_idle;
        total_prev = cpu[0].idle + cpu[0].non_idle;
        totald = total - total_prev;
        idled = cpu[1].idle - cpu[0].idle;
        cpu_usage_pct = (totald - idled) / totald;

        result.push_back(cpu_usage_pct);
    }

    this->cpu_result_ = result;
    now = std::time(nullptr);
    this->result_updated_ = now;
}

void Processor::AddCpuSample(int cpu_id, int idle, int non_idle) {
    size_t count = cpu_id;
    std::time_t now;
    CpuNData data = {idle, non_idle};

    if (count + 1 > cpu_data_.size()) {
        // Since a vector for this CPUn doesn't exist, it has no data yet.
        // So we need to first create a new CPUn vector, and prime its CpuNData
        // vector.
        std::vector<CpuNData> new_cpu{data};
        this->cpu_data_.push_back(new_cpu);
        this->cpu_data_[cpu_id].push_back(data);
    } else {
        this->cpu_data_[cpu_id].erase(this->cpu_data_[cpu_id].begin());
        this->cpu_data_[cpu_id].push_back(data);
    }

    now = std::time(nullptr);
    this->result_updated_ = now;
}

void Processor::PrintData() {
    int count{0};
    for (std::vector<CpuNData> cpu : this->cpu_data_) {
        std::cout << "CPU" << std::to_string(count)
                  << " idle: " << std::to_string(cpu[1].idle)
                  << " non_idle: " << std::to_string(cpu[1].non_idle)
                  << "\n    prev_idle: " << std::to_string(cpu[0].idle)
                  << " prev_non_idle: " << std::to_string(cpu[0].non_idle)
                  << "\n";
        count++;
    }
}