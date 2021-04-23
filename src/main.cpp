#include <sys/time.h>
#include <unistd.h>

#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "format.h"
#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include "process.h"

int main() {
    System system;
    //std::vector<Process> &processes = system.Processes();
    //std::this_thread::sleep_for(std::chrono::seconds(4));
    NCursesDisplay::Display(system, 15);
    //std::string line = "559 (cinnamon-sessio) S 1501 1559 1559 0 -1 4194560 13207 102355 8 54 22 6 555 109 20 0 4 0 2377 385671168 6630 18446744073709551615 94430770675712 94430770936309 140726453169248 0 0 0 0 4097 81922 0 0 0 17 1 0 0 0 0 0 94430771042064 94430771057300 94430803120128 140726453176972 140726453177008 140726453177008 140726453178334 0";
    //std::cout << LinuxParser::NthToken(line, 1) << "\n";
	//Process process(1);
	//float cpu = process.CpuUtilization();
	//std::cout << LinuxParser::User(1000) << "\n";
    //Process process = Process(9365);
    
    //std::cout << process.Ram() << "\n";
    //std::cout << process.Command() << "\n";
 //    for (int i = 0; i < 10; i++) {
     // 	float val = process.CpuUtilization();
    	// std::this_thread::sleep_for(std::chrono::seconds(8));
    	// val = process.CpuUtilization();
     	//std::cout << std::to_string(process.UpTime()) << "\n";
	// }
	// while (1) {
	// 	for (Process process : processes)
	// 	{
	// 		process.CpuUtilization();
	// 		//std::this_thread::sleep_for(std::chrono::seconds(1));
	// 	}
	// }

}

// TODO: Add proper function comments