#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace Utils {
    std::string getCurrentDate();
    boost::gregorian::date getCurrentBoostDate();

    void clearScreen();
    void pause();
    int getChoice(int min, int max);
    void printHeader(const std::string& title);
}
#endif