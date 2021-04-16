#include <sys/time.h>
#include <unistd.h>

#include <ctime>
#include <iostream>
#include <string>

#include "format.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
    System system;
    system.Cpu().UpdateData();
    system.Cpu().PrintData();
    sleep(5);
    system.Cpu().UpdateData();
    system.Cpu().PrintData();

    system.Cpu().UpdateResult();

    for (auto cpu_pct : system.Cpu().Utilization()) {
        std::cout << std::to_string(cpu_pct) << "\n";
    }
    std::cout << "Done.\n";
    while (1);
}