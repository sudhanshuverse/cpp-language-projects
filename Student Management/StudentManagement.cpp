#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <regex>
using namespace std;

// Student structure
struct Student
{
    int roll;
    char name[50];
    char course[30];
    char year[10];
};

// Utility function for clearing input buffer
void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Color codes
string red = "\033[1;31m";
string green = "\033[1;32m";
string yellow = "\033[1;33m";
string blue = "\033[1;34m";
string magenta = "\033[1;35m";
string cyan = "\033[1;36m";
string reset = "\033[0m";

// Input validation
bool isAlphaString(const string &str)
{
    return regex_match(str, regex("^[A-Za-z ]+$"));
}

bool isAlnumString(const string &str)
{
    return regex_match(str, regex("^[A-Za-z0-9 ]+$"));
}

bool isValidYear(const string &str)
{
    return regex_match(str, regex("^(20[0-9]{2}|FY|SY|TY)$")); // e.g., 2024 or FY
}

// Function to add student
void add()
{
    Student s;
    string temp;

    // Input Roll Number
    while (true)
    {
        cout << cyan << "Enter Roll Number (digits only): " << reset;
        cin >> s.roll;
        if (cin.fail() || s.roll <= 0)
        {
            cout << red << "Invalid input! Roll must be a positive number.\n"<< reset;
            clearInput();
        }
        else
        {
            clearInput();
            break;
        }
    }

    // Input Name
    while (true)
    {
        cout << cyan << "Enter Full Name (letters and spaces only, max 50 chars): " << reset;
        getline(cin, temp);
        if (!isAlphaString(temp) || temp.length() >= 50)
        {
            cout << red << "Invalid name format!\n"<< reset;
        }
        else
        {
            strcpy(s.name, temp.c_str());
            break;
        }
    }

    // Input Course
    while (true)
    {
        cout << cyan << "Enter Course (e.g., BCA, BScIT, max 30 chars): " << reset;
        getline(cin, temp);
        if (!isAlnumString(temp) || temp.length() >= 30)
        {
            cout << red << "Invalid course format!\n"<< reset;
        }
        else
        {
            strcpy(s.course, temp.c_str());
            break;
        }
    }

    // Input Year
    while (true)
    {
        cout << cyan << "Enter Year (e.g., FY, SY, TY, or 2025): " << reset;
        getline(cin, temp);
        if (!isValidYear(temp) || temp.length() >= 10)
        {
            cout << red << "Invalid year format!\n"<< reset;
        }
        else
        {
            strcpy(s.year, temp.c_str());
            break;
        }
    }

    // Write to file
    ofstream file("student.dat", ios::binary | ios::app);
    if (!file)
    {
        cout << red << "Error opening file!\n"<< reset;
        return;
    }

    file.write((char *)&s, sizeof(s));
    file.close();

    cout << green << "Student record added successfully!\n"<< reset;
}

// View student records
void view()
{
    Student s;
    ifstream file("student.dat", ios::binary);

    // Check if file can't be opened or is empty
    if (!file || file.peek() == ifstream::traits_type::eof())
    {
        cout << red << "No student records found in the database.\n" << reset;
        return;
    }

    bool isEmpty = true;

    // Loop to read records
    while (file.read((char *)&s, sizeof(s)))
    {
        if (isEmpty)
        {
            // Print header ONLY once when first record is read
            cout << blue << "\n---- All Student Records ----\n" << reset;
        }

        isEmpty = false;

        cout << yellow << "Roll: " << reset << s.roll << "\n";
        cout << yellow << "Name: " << reset << s.name << "\n";
        cout << yellow << "Course: " << reset << s.course << "\n";
        cout << yellow << "Year: " << reset << s.year << "\n";
        cout << magenta << "---------------------------\n" << reset;
    }

    if (isEmpty)
    {
        cout << red << "No student records found in the database.\n" << reset;
    }

    file.close();
}


// Delete student record
void del()
{
    int roll;
    bool found = false;

    cout << cyan << "Enter Roll Number to delete: " << reset;
    cin >> roll;
    clearInput();

    ifstream file("student.dat", ios::binary);
    ofstream temp("temp.dat", ios::binary);

    Student s;

    while (file.read((char *)&s, sizeof(s)))
    {
        if (s.roll != roll)
        {
            temp.write((char *)&s, sizeof(s));
        }
        else
        {
            found = true;
        }
    }

    file.close();
    temp.close();
    remove("student.dat");
    rename("temp.dat", "student.dat");

    if (found)
        cout << green << "Record deleted successfully.\n"<< reset;
    else
        cout << red << "Record not found!\n"<< reset;
}

// Update student record
void update()
{
    int roll;
    bool found = false;

    cout << cyan << "Enter Roll Number to update: " << reset;
    cin >> roll;
    clearInput();

    fstream file("student.dat", ios::binary | ios::in | ios::out);
    Student s;
    string temp;

    while (file.read((char *)&s, sizeof(s)))
    {
        if (s.roll == roll)
        {
            found = true;

            cout << green << "✔ Record found. Enter new details:\n"<< reset;

            // Name
            while (true)
            {
                cout << cyan << "Enter Full Name (letters and spaces only): " << reset;
                getline(cin, temp);
                if (!isAlphaString(temp) || temp.length() >= 50)
                {
                    cout << red << "Invalid name format!\n"<< reset;
                }
                else
                {
                    strcpy(s.name, temp.c_str());
                    break;
                }
            }

            // Course
            while (true)
            {
                cout << cyan << "Enter Course (e.g., BCA): " << reset;
                getline(cin, temp);
                if (!isAlnumString(temp) || temp.length() >= 30)
                {
                    cout << red << "Invalid course format!\n"<< reset;
                }
                else
                {
                    strcpy(s.course, temp.c_str());
                    break;
                }
            }

            // Year
            while (true)
            {
                cout << cyan << "Enter Year (FY/SY/TY/2025): " << reset;
                getline(cin, temp);
                if (!isValidYear(temp) || temp.length() >= 10)
                {
                    cout << red << "Invalid year format!\n"<< reset;
                }
                else
                {
                    strcpy(s.year, temp.c_str());
                    break;
                }
            }

            // Go back and update
            file.seekp(-sizeof(s), ios::cur);
            file.write((char *)&s, sizeof(s));

            cout << green << "Record updated successfully.\n"<< reset;
            break;
        }
    }

    file.close();

    if (!found)
        cout << red << "Record not found!\n"<< reset;
}

// Main menu
int main()
{
    while (true)
    {
        int choice;

        cout << blue << "\n===== Student Management System =====\n"<< reset;
        cout << green << "1. Add Student\n"<< reset;
        cout << red << "2. Delete Student\n"<< reset;
        cout << yellow << "3. Update Record\n"<< reset;
        cout << cyan << "4. View All Students\n"<< reset;
        cout << magenta << "5. Exit\n"<< reset;
        cout << blue << "=====================================\n"<< reset;

        cout << "Choose an option (1-5): ";
        cin >> choice;
        clearInput();

        switch (choice)
        {
        case 1:
            add();
            break;
        case 2:
            del();
            break;
        case 3:
            update();
            break;
        case 4:
            view();
            break;
        case 5:
            cout << red << "Exiting program... Goodbye!\n"<< reset;
            return 0;
        default:
            cout << red << "Invalid choice! Please enter a number between 1 and 5.\n"<< reset;
        }
    }
}
