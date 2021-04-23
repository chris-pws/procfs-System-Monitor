#include "format.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using std::string;

string Format::ElapsedTime(long seconds) {
    std::vector<long> time_denom{3600, 60, 1};
    string time_formatted = "00:00:00";
    long time_num = seconds;
    std::ldiv_t dv{};
    int days;

    if (time_num < 0) {
    	return time_formatted;
    } else {
    	time_formatted = "";
    }

    dv = std::div(time_num, 86400l);
    days = dv.quot;
    if (days > 0) {
	    time_formatted = std::to_string(days) + " day";
	    if (days > 1) {
	    	time_formatted += "s";
	    }
	    time_formatted += ", ";
	    time_num = dv.rem;
	}

    for (int i = 0; i < 3; i++) {
        dv = std::div(time_num, time_denom[i]);
        if (dv.quot < 10) {
            time_formatted.append("0");
        }
        time_formatted = time_formatted + std::to_string(dv.quot) + ":";
        time_num = dv.rem;
    }

    time_formatted.pop_back();
    return time_formatted;
}