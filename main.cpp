#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <exception>
#include <set>
#include <iomanip>

using namespace std;

class Date {
public:
    Date(int new_year, int new_month, int new_day) {
        year = new_year;
        month = new_month;
        day = new_day;
    }

    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
private:
    int year;
    int month;
    int day;
};

bool operator < (const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() < rhs.GetYear()) {
        return true;
    }
    else if (lhs.GetMonth() < rhs.GetMonth()) {
        if (lhs.GetYear() > rhs.GetYear()) {
            return false;
        }
        return true;
    }
    else if (lhs.GetDay() < rhs.GetDay()) {
        if (lhs.GetYear() > rhs.GetYear() || lhs.GetMonth() > rhs.GetMonth()) {
            return false;
        }
        return true;
    }
    else if (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() == rhs.GetDay()){
        return false;
    }
    else {
        return false;
    }
}

bool operator == (const Date& lhs, const Date& rhs) {
    return (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() == rhs.GetDay());
}

ostream& operator << (ostream& stream, const Date& date) {
    stream << setfill('0') << fixed << setw(4) << date.GetYear() << "-" << setw(2) << date.GetMonth() << "-" << setw(2) << date.GetDay();
    return stream;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        events[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const string& event) {
        if (events[date].count(event)) {
            events[date].erase(event);
            return true;
        }
        else {
            return false;
        }
    }

    int DeleteDate(const Date& date) {
        if (events.count(date)) {
            int n = events[date].size();
            events.erase(date);
            return n;
        }
        else {
            return 0;
        }
    }

    void Find(const Date& date) const {
        if (events.count(date)) {
            for (auto& x : events) {
                if (x.first == date) {
                    for (auto& y : x.second) {
                        cout << y << endl;
                    }
                    break;
                }
            }
        }
    }

    void Print() const {
        for (auto& x : events) {
            for (auto& y : x.second) {
                cout << x.first << " " << y << endl;
            }
        }
    }
private:
    map <Date, set<string>> events;
};

void ParseData(const string& data, Database& db) {
    stringstream ss(data);
    string request, date, event;
    ss >> request >> date >> event;

    stringstream s_date(date);

    if (request == "Print") {
        db.Print();
    }
    else if (request != "Add" && request != "Find" && request != "Del") {
        throw invalid_argument("Unknown command: " + request);
    }
    else {
        int year, month, day;
        s_date >> year;
        if (s_date.peek() == '-') {
            s_date.ignore(1);
            s_date >> month;
            if (s_date.peek() == '-') {
                s_date.ignore(1);
                s_date >> day;
            }
        }

        if (s_date.fail() || s_date.peek() != EOF) {
            throw invalid_argument("Wrong date format: " + date);
        }
        else if (month < 1 || month > 12) {
            throw invalid_argument("Month value is invalid: " + to_string(month));
        }
        else if (day < 1 || day > 31) {
            throw invalid_argument("Day value is invalid: " + to_string(day));
        }

        if (request == "Add") {
            db.AddEvent({ year, month, day }, event);
        }
        else if (request == "Find") {
            db.Find({ year, month, day });
        }
        else if (request == "Del" && event.empty()) {
            int n = db.DeleteDate({ year, month, day });
            cout << "Deleted " << n << " events\n";
        }
        else if (request == "Del") {
            bool tmp = db.DeleteEvent({ year, month, day }, event);
            if (tmp) {
                cout << "Deleted successfully\n";
            }
            else {
                cout << "Event not found\n";
            }
        }
    }
}

int main() {
    Database db;

    try {
        string command;
        while (getline(cin, command)) {
            if (!command.empty()) {
                ParseData(command, db);
            }
        }
    }
    catch (invalid_argument& i) {
        cout << i.what();
    }

    return 0;
}
