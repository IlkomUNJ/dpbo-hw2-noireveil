#include "Utils.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <limits>
#include <boost/date_time/gregorian/gregorian.hpp>

// Fungsi string date
std::string Utils::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

// Fungsi untuk Boost date
boost::gregorian::date Utils::getCurrentBoostDate() {
    return boost::gregorian::day_clock::local_day();
}


void Utils::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Utils::pause() {
    std::cout << "\nTekan Enter untuk melanjutkan...";
    std::cin.get(); 
}

int Utils::getChoice(int min, int max) {
    int choice;
    while (true) {
        std::cout << "Pilihan Anda (" << min << "-" << max << "): ";
        std::cin >> choice;
        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input tidak valid. Coba lagi.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}

void Utils::printHeader(const std::string& title) {
    clearScreen();
    std::cout << "--- " << title << " ---\n\n";
}