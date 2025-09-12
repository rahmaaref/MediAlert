#include <iostream>
#include <unordered_map>
#include <fstream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <Medicine.h>

using namespace std;

// Use a hash-based container for O(1) lookup/removal
unordered_multimap<string, Medicine> medicines;

void saveToFile()
{
    ofstream file("medicines.txt");
    for (auto &p : medicines)
    {
        file << p.second.getName() << "," << p.second.getTime() << endl;
    }
}

void loadFromFile()
{
    ifstream file("medicines.txt");
    string line;
    while (getline(file, line))
    {
        size_t pos = line.find(",");
        if (pos != string::npos)
        {
            string name = line.substr(0, pos);
            string timeStr = line.substr(pos + 1);
            medicines.emplace(name, Medicine(name, timeStr));
        }
    }
}

void showNotification(const string &title, const string &message)
{
    MessageBoxA(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
}

void checkReminders()
{
    while (true)
    {
        for (auto &p : medicines)
        {
            if (p.second.isTimeToTake())
            {
                showNotification("MediAlert", "Time to take: " + p.second.getName());
                this_thread::sleep_for(chrono::seconds(60)); // avoid repeat in same minute
            }
        }
        this_thread::sleep_for(chrono::seconds(30));
    }
}

void menu()
{
    int choice;
    do
    {
        cout << "\n=== MediAlert Menu ===\n";
        cout << "1. Add Medicine\n";
        cout << "2. View Medicines\n";
        cout << "3. Remove Medicine\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string name, time;
            cout << "Enter medicine name: ";
            cin >> name;
            cout << "Enter time (HH:MM): ";
            cin >> time;
            medicines.emplace(name, Medicine(name, time));
            saveToFile();
        }
        else if (choice == 2)
        {
            for (auto &p : medicines)
            {
                cout << p.second.getName() << " at " << p.second.getTime() << endl;
            }
        }
        else if (choice == 3)
        {
            string name;
            cout << "Enter medicine name: ";
            cin >> name;

            auto range = medicines.equal_range(name);
            int cnt = distance(range.first, range.second);

            if (cnt == 0)
            {
                cout << "No medicine found with that name.\n";
            }
            else
            {
                medicines.erase(range.first, range.second);
                cout << "Removed all medicine(s) named " << name << ".\n";
            }

            saveToFile();
        }

    } while (choice != 4);
}

int main()
{
    loadFromFile();

    thread reminderThread(checkReminders);
    reminderThread.detach();

    menu();

    return 0;
}
