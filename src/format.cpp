#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string res;
    int s, m, h;
    string str_s, str_m, str_h;

    h = seconds / 60 / 60;
    seconds = seconds - (h * 60 * 60);
    str_h = std::to_string(h);
    if (str_h.size() == 1) {
        str_h = "0" + str_h;
    }

    m = seconds / 60;
    seconds = seconds - (m * 60);
    str_m = std::to_string(m);
    if (str_m.size() == 1) {
        str_m = "0" + str_m;
    }

    s = seconds;
    str_s = std::to_string(s);
    if (str_s.size() == 1) {
        str_s = "0" + str_s;
    }

    res = str_h + ":" + str_m + ":" + str_s;
    return res;

}