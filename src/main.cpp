#include <sys/time.h>

#include <ctime>
#include <iostream>
#include <string>
#include <unistd.h>

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
    std::cout << "Done.\n";
    while (1);
}