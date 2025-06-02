#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

// ANSI color codes
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";

// Helper function for validated integer input within range
int getValidatedInt(const string &prompt, int minVal = numeric_limits<int>::min(), int maxVal = numeric_limits<int>::max()) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard leftover input
            return val;
        } else {
            cout << RED << "Invalid input. Please enter a number";
            if (minVal != numeric_limits<int>::min())
                cout << " >= " << minVal;
            if (maxVal != numeric_limits<int>::max())
                cout << " and <= " << maxVal;
            cout << "." << RESET << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

class Student {
private:
    int roll;
    string name;
    int totalDays;
    int presentDays;

public:
    Student() : roll(0), name(""), totalDays(0), presentDays(0) {}

    // Input student details with validation
    void inputStudent() {
        roll = getValidatedInt("Enter Roll No: ", 1);
        cout << "Enter Name: ";
        getline(cin, name);
        while (name.empty()) {
            cout << RED << "Name cannot be empty. Enter Name: " << RESET;
            getline(cin, name);
        }
        totalDays = 0;
        presentDays = 0;
    }

    int getRoll() const { return roll; }

    // Increment attendance counts
    void markPresent() {
        totalDays++;
        presentDays++;
    }
    void markAbsent() {
        totalDays++;
    }

    // Display student attendance details with color formatting
    void display() const {
        cout << CYAN << "Roll: " << roll
             << ", Name: " << name
             << ", Present: " << presentDays
             << ", Total: " << totalDays
             << ", Attendance: ";
        if (totalDays == 0)
            cout << "0.00%";
        else {
            double percent = (double)presentDays / totalDays * 100.0;
            cout << fixed << setprecision(2) << percent << "%";
        }
        cout << RESET << endl;
    }

    // Write student data to binary file stream
    void writeToFile(ostream &out) const {
        out.write(reinterpret_cast<const char *>(&roll), sizeof(roll));

        size_t length = name.size();
        out.write(reinterpret_cast<const char *>(&length), sizeof(length));
        out.write(name.c_str(), length);

        out.write(reinterpret_cast<const char *>(&totalDays), sizeof(totalDays));
        out.write(reinterpret_cast<const char *>(&presentDays), sizeof(presentDays));
    }

    // Read student data from binary file stream
    bool readFromFile(istream &in) {
        if (!in.read(reinterpret_cast<char *>(&roll), sizeof(roll)))
            return false;

        size_t length = 0;
        if (!in.read(reinterpret_cast<char *>(&length), sizeof(length)))
            return false;

        name.resize(length);
        if (!in.read(&name[0], length))
            return false;

        if (!in.read(reinterpret_cast<char *>(&totalDays), sizeof(totalDays)))
            return false;

        if (!in.read(reinterpret_cast<char *>(&presentDays), sizeof(presentDays)))
            return false;

        return true;
    }
};

// Find a student's position (byte offset) in the file, returns -1 if not found
long findStudentPosition(int roll) {
    ifstream in("students.dat", ios::binary);
    if (!in)
        return -1;

    Student s;
    while (true) {
        long pos = in.tellg();
        if (!s.readFromFile(in))
            break;
        if (s.getRoll() == roll) {
            return pos;
        }
    }
    return -1;
}

void addStudent() {
    Student s;
    s.inputStudent();

    // Check for duplicate roll number
    if (findStudentPosition(s.getRoll()) != -1) {
        cout << RED << "Student with this roll number already exists!" << RESET << endl;
        return;
    }

    ofstream out("students.dat", ios::binary | ios::app);
    s.writeToFile(out);
    cout << GREEN << "Student added successfully!" << RESET << endl;
}

void markAttendance(bool present) {
    int roll = getValidatedInt("Enter Roll No to mark attendance: ", 1);
    fstream file("students.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << RED << "No records found!" << RESET << endl;
        return;
    }

    Student s;
    bool found = false;
    long pos = 0;

    while (true) {
        pos = file.tellg();
        if (!s.readFromFile(file))
            break;
        if (s.getRoll() == roll) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << RED << "Student not found!" << RESET << endl;
        return;
    }

    if (present)
        s.markPresent();
    else
        s.markAbsent();

    // Seek back to the position to overwrite student record
    file.seekp(pos);
    s.writeToFile(file);

    cout << GREEN << "Attendance updated successfully!" << RESET << endl;
}

void viewAll() {
    ifstream in("students.dat", ios::binary);
    if (!in) {
        cout << RED << "No records found!" << RESET << endl;
        return;
    }
    Student s;
    cout << YELLOW << "==== Attendance Summary ====" << RESET << endl;
    bool any = false;
    while (s.readFromFile(in)) {
        s.display();
        any = true;
    }
    if (!any) {
        cout << RED << "No records found!" << RESET << endl;
    }
}

void searchByRoll() {
    int roll = getValidatedInt("Enter Roll No to search: ", 1);
    ifstream in("students.dat", ios::binary);
    if (!in) {
        cout << RED << "No records found!" << RESET << endl;
        return;
    }
    Student s;
    bool found = false;
    while (s.readFromFile(in)) {
        if (s.getRoll() == roll) {
            cout << YELLOW << "Student Found:" << RESET << endl;
            s.display();
            found = true;
            break;
        }
    }
    if (!found) {
        cout << RED << "Student not found!" << RESET << endl;
    }
}

int main() {
    while (true) {
        cout << CYAN << "\n===== Student Attendance Tracker =====\n" << RESET;
        cout << GREEN << "1. Add New Student\n" << RESET;
        cout << YELLOW << "2. Mark Present\n" << RESET;
        cout << MAGENTA << "3. Mark Absent\n" << RESET;
        cout << BLUE << "4. View All Attendance\n" << RESET;
        cout << CYAN << "5. Search by Roll No\n" << RESET;
        cout << RED << "6. Exit\n" << RESET;
        cout << "Enter choice: ";

        int choice = getValidatedInt("", 1, 6);

        switch (choice) {
        case 1:
            addStudent();
            break;
        case 2:
            markAttendance(true);
            break;
        case 3:
            markAttendance(false);
            break;
        case 4:
            viewAll();
            break;
        case 5:
            searchByRoll();
            break;
        case 6:
            cout << GREEN << "Exiting... Goodbye!" << RESET << endl;
            return 0;
        default:
            cout << RED << "Invalid choice, try again." << RESET << endl;
        }
    }
}
