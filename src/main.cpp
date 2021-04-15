#include <sys/time.h>

#include <ctime>
#include <iostream>
#include <string>

#include "format.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
    // System system;
    // NCursesDisplay::Display(system);
    /*
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    std::cout << std::to_string(current_time.tv_sec) << "."
              << std::to_string(current_time.tv_usec) << "\n";
    std::cout << Format::ElapsedTime(81224) << "\n";
    */
    std::string cpu = "cpu2";
    std::string other = "intr";

    if (cpu.find("cpu") != std::string::npos)
    {

    	std::cout << "Yes\n";
    }
    else
    {
    	std::cout << "No\n";
    }

}