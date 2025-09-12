#ifndef MEDICINE_H
#define MEDICINE_H

#include <string>

class Medicine {
private:
    std::string name;
    std::string timeStr; // "HH:MM"

public:
    Medicine(std::string n, std::string t);

    std::string getName() const;
    std::string getTime() const;

    bool isTimeToTake() const;
};

#endif
