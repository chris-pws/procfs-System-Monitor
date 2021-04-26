#include "format.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using std::string;

/********* Format::ElapsedTime *******
 *  Converts a value in second to human readable time in unit increments.
 *  Inputs: Long integer of seconds.
 *  Outputs: String in d:h:m:s format (day, hour, minute, second).
 */
string Format::ElapsedTime(long seconds) {
    std::vector<long> time_denom{86400, 3600, 60, 1};
    string time_formatted = "00:00:00:00";
    long time_num = seconds;
    std::ldiv_t dv{};

    if (time_num < 0) {
        return time_formatted;
    } else {
        time_formatted = "";
    }

    for (auto denom_secs : time_denom) {
        dv = std::div(time_num, denom_secs);
        if (dv.quot < 10) {
            time_formatted.append("0");
        }
        time_formatted = time_formatted + std::to_string(dv.quot) + ":";
        time_num = dv.rem;
    }

    time_formatted.pop_back();
    return time_formatted;
}