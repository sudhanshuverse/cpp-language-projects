#include <iostream>
#include <iomanip>
#include <string>
#include <regex>      // For input validation
using namespace std;

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

float getValidMark(const string& subjectName) {
    float mark;
    while (true) {
        cout << YELLOW << "Enter marks for " << subjectName << ": " << RESET;
        cin >> mark;

        if (cin.fail() || mark < 0 || mark > 100) {
            cin.clear(); // clear error flag
            cin.ignore(1000, '\n'); // discard invalid input
            cout << RED << "Invalid input! Enter marks between 0 and 100.\n" << RESET;
        } else {
            cin.ignore(1000, '\n'); // clear the buffer after valid input
            break;
        }
    }
    return mark;
}

string calculateGrade(float percentage) {
    if (percentage >= 90)
        return "A+";
    else if (percentage >= 80)
        return "A";
    else if (percentage >= 70)
        return "B+";
    else if (percentage >= 60)
        return "B";
    else if (percentage >= 50)
        return "C";
    else if (percentage >= 40)
        return "D";
    else
        return "F";
}

int main() {
    int subjects;
    string studentName;

    cout << CYAN << "==============================\n";
    cout << "     Student Grade Calculator\n";
    cout << "==============================\n" << RESET;

    cout << MAGENTA << "Enter student name: " << RESET;
    getline(cin, studentName);

    while (true) {
        cout << MAGENTA << "Enter number of subjects: " << RESET;
        cin >> subjects;

        if (cin.fail() || subjects <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << RED << "Invalid number! Enter a positive integer.\n" << RESET;
        } else {
            cin.ignore(1000, '\n');
            break;
        }
    }

    float totalMarks = 0.0;

    for (int i = 1; i <= subjects; i++) {
        string subjectName;
        regex validName("^[A-Za-z ]+$");

        while (true) {
            cout << CYAN << "\nSubject " << i << " Name: " << RESET;
            getline(cin, subjectName);

            if (regex_match(subjectName, validName)) {
                break;
            } else {
                cout << RED << "Invalid subject name! Use only letters and spaces.\n" << RESET;
            }
        }

        float marks = getValidMark(subjectName);
        totalMarks += marks;
    }

    float percentage = totalMarks / subjects;
    string grade = calculateGrade(percentage);

    cout << GREEN << "\n=========== RESULT ===========\n" << RESET;
    cout << CYAN << "Student Name: " << RESET << studentName << endl;
    cout << CYAN << "Total Marks : " << RESET << totalMarks << "/" << subjects * 100 << endl;
    cout << CYAN << "Percentage  : " << RESET << fixed << setprecision(2) << percentage << "%" << endl;
    cout << CYAN << "Grade       : " << RESET << grade << endl;
    cout << GREEN << "==============================\n" << RESET;

    return 0;
}
