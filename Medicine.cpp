#include "Medicine.h"
#include <ctime>
#include <sstream>

Medicine::Medicine(std::string n, std::string t) : name(n), timeStr(t) {}

std::string Medicine::getName() const {
    return name;
}

std::string Medicine::getTime() const {
    return timeStr;
}

bool Medicine::isTimeToTake() const {
    // Get current time in HH:MM
    time_t now = time(nullptr);
    struct tm* local = localtime(&now);

    std::ostringstream oss;
    oss << (local->tm_hour < 10 ? "0" : "") << local->tm_hour << ":"
        << (local->tm_min < 10 ? "0" : "") << local->tm_min;

    return oss.str() == timeStr;
}
