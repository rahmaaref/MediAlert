#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <windows.h>
#include "Medicine.h"

using namespace std;

vector<Medicine> medicines;

void saveToFile()
{
    ofstream file("medicines.txt");
    for (auto &m : medicines)
    {
        file << m.getName() << "," << m.getTime() << endl;
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
            medicines.push_back(Medicine(name, timeStr));
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
        for (auto &m : medicines)
        {
            if (m.isTimeToTake())
            {
                showNotification("MediAlert", "Time to take: " + m.getName());
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
            medicines.push_back(Medicine(name, time));
            saveToFile();
        }
        else if (choice == 2)
        {
            for (auto &m : medicines)
            {
                cout << m.getName() << " at " << m.getTime() << endl;
            }
        }
        else if (choice == 3)
        {
            string name;
            cout << "Enter medicine name: ";
            cin >> name;

            int cnt = 0;
            auto it = medicines.begin();
            while (!medicines.empty() && it != medicines.end())
            {
                if (it->getName() == name)
                {
                    it = medicines.erase(it); // safely erase and get next iterator
                    cnt++;
                }
                else
                {
                    ++it;
                }
            }

            saveToFile(); // save updated list

            if (cnt == 0)
            {
                cout << "No medicine found with that name.\n";
            }
            else
            {
                cout << "Removed all medicine(s) named " << name << ".\n";
            }
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
