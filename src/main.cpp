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
    NCursesDisplay::Display(system);
}