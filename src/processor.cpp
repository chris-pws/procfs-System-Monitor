#include "processor.h"
#include "linux_parser.h"

#include <sys/time.h>

#include <ctime>
#include <sstream>
#include <string>
#include <vector>

std::vector<float> Processor::Utilization() {
    
    std::time_t now = std::time(nullptr);

    if ((now - this->data_updated_ > DATA_DELTA) || (this->cpu_data_.empty()))
    {
    	UpdateData();
    }
    if ((now - this->result_updated_ > RESULT_DELTA) || (this->cpu_result_.empty()))
    {
    	UpdateResult();
    }

    std::vector<float> cpu_result{0.0f};
    return cpu_result;
    //return cpu_result_ // remove the above two lines and uncomment this when done
}

void Processor::UpdateData() {
	std::string line;
	std::string cpu = "0";
	std::string user = "0";
	std::string nice = "0";
	std::string system = "0";
	std::string idle = "0";
	std::string irq = "0";
	std::string soft_irq = "0";
	std::string steal = "0";
	std::string guest = "0";
	std::string guest_nice = "0";
	int state_value{0};
	int count{2};
	std::vector<int> cpu_state;

	std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
	if (filestream.is_open()) {
			while (std::getline(filestream, line))
			{
				if (line.find(cpu) == std::string::npos) {
					break;
				}

				std::stringstream ss(line);
				while ( ss >> state_value )
				{
					cpu_state.push_back(state_value);
				}


			}
	}
}

void Processor::UpdateResult() {

}

void Processor::AddCpuSample(int cpu_id, int idle, int non_idle){
	size_t count = cpu_id++;
	CpuNData data = { idle, non_idle };

	if (count > cpu_data_.size())
	{
		// No data has been collected yet, since a vector for this CPUn doesn't exist
		// So we need to first create a new CPUn vector, and add an additional CpuNData to it
		std::vector<CpuNData> new_cpu{data};
		cpu_data_.push_back(new_cpu);
		cpu_data_[cpu_id].push_back(data);
	}

	cpu_data_[cpu_id].erase(cpu_data_[cpu_id].begin());
	cpu_data_[cpu_id].push_back(data);
}