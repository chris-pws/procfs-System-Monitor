#include "ncurses_display.h"

#include <curses.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "system.h"

using std::string;
using std::to_string;

/********* NCursesDisplay::ProgressBar *******
 *  Provides a visual representation of the floating point value provided. 
 *  50 bars uniformly displayed from 0 - 100 %. 2% is one bar(|).
 *  Inputs: a floating point value.
 *  Outputs: None
 */
std::string NCursesDisplay::ProgressBar(float percent) {
    std::string result{"0%"};
    int size{50};
    float bars{percent * size};

    for (int i{0}; i < size; ++i) {
        result += i <= bars ? '|' : ' ';
    }

    string display{to_string(percent * 100).substr(0, 4)};
    if (percent < 0.1 || percent == 1.0)
        display = " " + to_string(percent * 100).substr(0, 3);
    return result + " " + display + "/100%";
}

/********* NCursesDisplay::DisplaySystem *******
 *  Displays system-related statistics using the ncurses display.
 *  Inputs: None.
 *  Outputs: None.
 */
void NCursesDisplay::DisplaySystem(System &system, WINDOW *window) {
    int row{0};
    int cpu_id{0};

    mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
    mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
    for (float cpu : system.Cpu().Utilization()) {
        wattron(window, COLOR_PAIR(CpuRowColor(row)));
        mvwprintw(
            window, ++row, 2,
            std::string(std::string("CPU") + std::to_string(cpu_id) + ": ")
                .c_str());
        mvwprintw(window, row, 10, "");
        wprintw(window, ProgressBar(cpu).c_str());
        wattroff(window, COLOR_PAIR(CpuRowColor(row)));
        cpu_id++;
    }
    wattron(window, COLOR_PAIR(3));
    mvwprintw(window, ++row, 2, "Memory: ");
    mvwprintw(window, row, 10, "");
    wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
    wattroff(window, COLOR_PAIR(3));
    mvwprintw(
        window, ++row, 2,
        ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
    mvwprintw(
        window, ++row, 2,
        ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
    mvwprintw(window, ++row, 2,
              ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
    wrefresh(window);
}

/********* NCursesDisplay::DisplayProcesses *******
 *  Displays interesting values for the top processes running on the system.
 *  Inputs: None.
 *  Outputs: None.
 */
void NCursesDisplay::DisplayProcesses(std::vector<Process> &processes,
                                      WINDOW *window, int n) {
    int row{0};
    int count{1};
    int const pid_column{2};
    int const user_column{9};
    int const cpu_column{20};
    int const ram_column{27};
    int const time_column{37};
    int const command_column{52};

    wattron(window, COLOR_PAIR(3));
    mvwprintw(window, ++row, pid_column, "PID");
    mvwprintw(window, row, user_column, "USER");
    mvwprintw(window, row, cpu_column, "CPU[%%]");
    mvwprintw(window, row, ram_column, "   RAM");
    mvwprintw(window, row, time_column, "TIME[d:h:m:s]");
    mvwprintw(window, row, command_column, "COMMAND");
    wattroff(window, COLOR_PAIR(3));
    int const num_processes = int(processes.size()) > n ? n : processes.size();
    for (Process process : processes) {
        if (count > num_processes) break;
        mvwprintw(window, ++row, pid_column, to_string(process.Pid()).c_str());
        mvwprintw(window, row, user_column,
                  process.User().substr(0, 10).c_str());
        mvwprintw(window, row, cpu_column, process.CpuPretty(2).c_str());
        mvwprintw(window, row, ram_column, process.Ram().c_str());
        mvwprintw(window, row, time_column,
                  Format::ElapsedTime(process.UpTime()).c_str());
        mvwprintw(window, row, command_column,
                  process.Command().substr(0, window->_maxx - 52).c_str());
        count++;
    }
}

/********* NCursesDisplay::Display *******
 *  Sets up ncurses and orchestrates rendering the system and process windows.
 *  Inputs: A system object, int as the number of lines to render.
 *  Outputs: None
 */
void NCursesDisplay::Display(System &system, int n) {
    initscr();      // start ncurses
    noecho();       // do not print input values
    cbreak();       // terminate ncurses on ctrl + c
    start_color();  // enable color
    curs_set(0);    // disable cursor
    std::vector<Process> &processes = system.Processes();

    int x_max{getmaxx(stdscr)};
    WINDOW *system_window = newwin(8 + system.Cpu().NumCpus(), x_max - 1, 0, 0);
    WINDOW *process_window =
        newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

    while (1) {
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_CYAN, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        box(system_window, 0, 0);
        box(process_window, 0, 0);
        DisplaySystem(system, system_window);
        ClearProcessWindow(process_window, n);
        DisplayProcesses(processes, process_window, n);
        wrefresh(system_window);
        wrefresh(process_window);
        refresh();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        processes = system.Processes();
    }
    endwin();
}

/********* NCursesDisplay::ClearProcessWindow *******
 *  Clears every line in the process window between each update.
 *  Inputs: Pointer to the ncurses window object, and an int of the number of 
 *          display lines
 *  Outputs: None
 */
void NCursesDisplay::ClearProcessWindow(WINDOW *window, int n) {
    int row{0};
    std::string clear(window->_maxx - 1, ' ');

    for (int i = 0; i < n + 1; i++) {
        mvwprintw(window, ++row, 1, clear.c_str());
    }
}

/********* NCursesDisplay::CpuRowColor *******
 *  Provides an alternating color scheme for multiple CPU usage rows.
 *  Inputs: The row in the ncurses system window currently being rendered.
 *  Outputs: An int for the window attribute color_pair parameter.
 */
int NCursesDisplay::CpuRowColor(int row) {
    if (row % 2 == 0) {
        return 1;
    } else {
        return 2;
    }
}