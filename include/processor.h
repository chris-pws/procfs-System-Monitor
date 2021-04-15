#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <ctime>
#include <vector>

#define DATA_DELTA 1
#define RESULT_DELTA 2

struct CpuNData {
    int idle;
    int non_idle;
};

class Processor {
   public:
    std::vector<float> Utilization();
    void UpdateData();
    void UpdateResult();

   private:
    // By abstracting this action out of UpdateData(), CPU sampling structure is
    // easily changed later.
    void AddCpuSample(int cpu_id, int idle, int non_idle);

    // Each vector represents CPU n, with a nested vector of CPU samples.
    std::vector<std::vector<CpuNData>> cpu_data_;
    // Each float in this vector represents the percentage of active CPU.
    std::vector<float> cpu_result_;
    // Update the cpu_result_ every x seconds.
    std::time_t data_updated_{0};

    std::time_t result_updated_{0};
};

#endif