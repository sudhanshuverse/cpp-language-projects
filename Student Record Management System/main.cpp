#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

class Student {
private:
    int id;
    string name;
    float marks[5];
    float average;
    char grade;

    void calculateGrade() {
        average = 0;
        for (int i = 0; i < 5; ++i)
            average += marks[i];
        average /= 5;

        if (average >= 90) grade = 'A';
        else if (average >= 75) grade = 'B';
        else if (average >= 60) grade = 'C';
        else if (average >= 45) grade = 'D';
        else grade = 'F';
    }

public:
    Student() {
        id = 0;
        name = "";
        average = 0;
        grade = 'F';
        for (int i = 0; i < 5; i++) marks[i] = 0;
    }

    void input() {
        cout << CYAN << "Enter ID: " << RESET;
        cin >> id;
        cin.ignore();
        cout << CYAN << "Enter Name: " << RESET;
        getline(cin, name);
        cout << CYAN << "Enter 5 subject marks: " << RESET;
        for (int i = 0; i < 5; ++i)
            cin >> marks[i];
        calculateGrade();
    }

    void display() const {
        cout << MAGENTA << "\n------------------------------\n" << RESET;
        cout << GREEN << "ID      : " << RESET << id << "\n";
        cout << GREEN << "Name    : " << RESET << name << "\n";
        cout << GREEN << "Marks   : " << RESET;
        for (int i = 0; i < 5; ++i) cout << marks[i] << " ";
        cout << "\n" << GREEN << "Average : " << RESET << average << "\n";
        cout << GREEN << "Grade   : " << RESET << grade << "\n";
        cout << MAGENTA << "------------------------------\n" << RESET;
    }

    int getID() const { return id; }

    void writeToFile() const {
        ofstream out("students.dat", ios::binary | ios::app);
        out.write((char*)this, sizeof(Student));
        out.close();
    }

    static void readAll() {
        Student s;
        ifstream in("students.dat", ios::binary);
        if (!in) {
            cout << RED << "No records found.\n" << RESET;
            return;
        }
        while (in.read((char*)&s, sizeof(Student))) {
            s.display();
        }
        in.close();
    }

    static void deleteByID(int del_id) {
        Student s;
        ifstream in("students.dat", ios::binary);
        ofstream out("temp.dat", ios::binary);
        bool found = false;

        while (in.read((char*)&s, sizeof(Student))) {
            if (s.getID() != del_id)
                out.write((char*)&s, sizeof(Student));
            else
                found = true;
        }

        in.close();
        out.close();
        remove("students.dat");
        rename("temp.dat", "students.dat");

        if (found)
            cout << GREEN << "Student deleted successfully.\n" << RESET;
        else
            cout << RED << "Student not found.\n" << RESET;
    }
};

// Utility function
void menu() {
    cout << YELLOW << "\n===== Student Record System =====\n" << RESET;
    cout << CYAN << "1. Add Student\n";
    cout << "2. Display All Students\n";
    cout << "3. Delete Student\n";
    cout << "4. Exit\n" << RESET;
    cout << YELLOW << "Enter your choice: " << RESET;
}

int main() {
    int choice, id;

    do {
        menu();
        cin >> choice;

        switch (choice) {
        case 1: {
            Student s;
            s.input();
            s.writeToFile();
            cout << GREEN << "Student record added.\n" << RESET;
            break;
        }
        case 2:
            Student::readAll();
            break;
        case 3:
            cout << CYAN << "Enter Student ID to delete: " << RESET;
            cin >> id;
            Student::deleteByID(id);
            break;
        case 4:
            cout << MAGENTA << "Exiting... Goodbye!\n" << RESET;
            break;
        default:
            cout << RED << "Invalid choice. Try again.\n" << RESET;
        }

    } while (choice != 4);

    return 0;
}
