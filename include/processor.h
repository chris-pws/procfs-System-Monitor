#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <ctime>
#include <vector>

#define DATA_DELTA 1
#define RESULT_DELTA 2

struct CpuNData {
    int idle;
    int active;
};

class Processor {
   public:
    std::vector<float> Utilization();
    void UpdateData();
    void UpdateResult();
    void PrintData();
    int NumCpus();

   private:
    void AddCpuSample(int cpu_id, int idle, int active);
    // Each vector represents CPU n, with a nested vector of CPU samples.
    std::vector<std::vector<CpuNData>> cpu_data_;
    // Each float represents the percentage of active time for CPU n.
    std::vector<float> cpu_result_;
    // Timestamps supporting cached updates of data and results.
    std::time_t data_updated_{0};
    std::time_t result_updated_{0};
};

#endif