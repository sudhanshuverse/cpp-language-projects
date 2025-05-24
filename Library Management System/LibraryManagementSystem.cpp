#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <regex>
#include <algorithm>

using namespace std;

struct Book {
    int id;
    char title[100];
    char author[50];
    char publisher[50];
    int year;
};

const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string YELLOW = "\033[1;33m";
const string BLUE = "\033[1;34m";
const string MAGENTA = "\033[1;35m";
const string CYAN = "\033[1;36m";
const string RESET = "\033[0m";

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isAlphaString(const string& str) {
    return regex_match(str, regex("^[A-Za-z ]+$"));
}

bool isAlphaNumString(const string& str) {
    return regex_match(str, regex("^[A-Za-z0-9 .,]+$"));
}

bool isValidYear(int year) {
    return year > 1440 && year <= 2025;
}

string toLower(const string& s) {
    string lower = s;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

bool confirmAction(const string& message) {
    char choice;
    cout << YELLOW << message << " (Y/N): " << RESET;
    cin >> choice;
    clearInput();
    return (choice == 'Y' || choice == 'y');
}

void addBook() {
    Book b;
    string temp;

    cout << CYAN << "\n----- Add New Book -----\n" << RESET;

    while (true) {
        cout << "Enter Book ID (positive integer): ";
        cin >> b.id;
        if (cin.fail() || b.id <= 0) {
            cout << RED << "Invalid input! Book ID must be a positive integer.\n" << RESET;
            clearInput();
        } else {
            clearInput();
            break;
        }
    }

    while (true) {
        cout << "Enter Book Title (max 100 chars): ";
        getline(cin, temp);
        if (temp.empty() || temp.length() >= 100) {
            cout << RED << "Invalid title!\n" << RESET;
        } else {
            strcpy(b.title, temp.c_str());
            break;
        }
    }

    while (true) {
        cout << "Enter Author Name (letters only): ";
        getline(cin, temp);
        if (!isAlphaString(temp) || temp.length() >= 50) {
            cout << RED << "Invalid author name!\n" << RESET;
        } else {
            strcpy(b.author, temp.c_str());
            break;
        }
    }

    while (true) {
        cout << "Enter Publisher Name (alphanumeric): ";
        getline(cin, temp);
        if (!isAlphaNumString(temp) || temp.length() >= 50) {
            cout << RED << "Invalid publisher!\n" << RESET;
        } else {
            strcpy(b.publisher, temp.c_str());
            break;
        }
    }

    while (true) {
        cout << "Enter Year of Publication: ";
        int year;
        cin >> year;
        clearInput();
        if (!isValidYear(year)) {
            cout << RED << "Invalid year!\n" << RESET;
        } else {
            b.year = year;
            break;
        }
    }

    ofstream file("library.dat", ios::binary | ios::app);
    file.write((char*)&b, sizeof(b));
    file.close();

    cout << GREEN << "Book record added successfully!\n" << RESET;
}

void viewBooks() {
    Book b;
    ifstream file("library.dat", ios::binary);
    if (!file || file.peek() == ifstream::traits_type::eof()) {
        cout << RED << "No book records found.\n" << RESET;
        return;
    }

    cout << BLUE << "\n----- All Book Records -----\n" << RESET;
    while (file.read((char*)&b, sizeof(b))) {
        cout << YELLOW << "Book ID: " << RESET << b.id << "\n";
        cout << YELLOW << "Title: " << RESET << b.title << "\n";
        cout << YELLOW << "Author: " << RESET << b.author << "\n";
        cout << YELLOW << "Publisher: " << RESET << b.publisher << "\n";
        cout << YELLOW << "Year: " << RESET << b.year << "\n";
        cout << MAGENTA << "----------------------------\n" << RESET;
    }
    file.close();
}

void deleteBook() {
    int id;
    bool found = false;

    cout << CYAN << "\n----- Delete Book -----\n" << RESET;
    cout << "Enter Book ID to delete: ";
    cin >> id;
    clearInput();

    ifstream file("library.dat", ios::binary);
    ofstream temp("temp.dat", ios::binary);
    Book b;

    while (file.read((char*)&b, sizeof(b))) {
        if (b.id != id) {
            temp.write((char*)&b, sizeof(b));
        } else {
            found = true;
        }
    }
    file.close();
    temp.close();

    if (!found) {
        cout << RED << "Book record not found!\n" << RESET;
        remove("temp.dat");
        return;
    }

    if (confirmAction("Are you sure you want to delete this book?")) {
        remove("library.dat");
        rename("temp.dat", "library.dat");
        cout << GREEN << "Book record deleted successfully.\n" << RESET;
    } else {
        remove("temp.dat");
        cout << YELLOW << "Deletion cancelled.\n" << RESET;
    }
}

void updateBook() {
    int id;
    bool found = false;

    cout << CYAN << "\n----- Update Book -----\n" << RESET;
    cout << "Enter Book ID to update: ";
    cin >> id;
    clearInput();

    fstream file("library.dat", ios::binary | ios::in | ios::out);
    Book b;
    string temp;

    while (file.read((char*)&b, sizeof(b))) {
        if (b.id == id) {
            found = true;

            while (true) {
                cout << "Enter new Title: ";
                getline(cin, temp);
                if (temp.empty() || temp.length() >= 100) {
                    cout << RED << "Invalid title!\n" << RESET;
                } else {
                    strcpy(b.title, temp.c_str());
                    break;
                }
            }

            while (true) {
                cout << "Enter new Author: ";
                getline(cin, temp);
                if (!isAlphaString(temp) || temp.length() >= 50) {
                    cout << RED << "Invalid author!\n" << RESET;
                } else {
                    strcpy(b.author, temp.c_str());
                    break;
                }
            }

            while (true) {
                cout << "Enter new Publisher: ";
                getline(cin, temp);
                if (!isAlphaNumString(temp) || temp.length() >= 50) {
                    cout << RED << "Invalid publisher!\n" << RESET;
                } else {
                    strcpy(b.publisher, temp.c_str());
                    break;
                }
            }

            while (true) {
                cout << "Enter new Year: ";
                int year;
                cin >> year;
                clearInput();
                if (!isValidYear(year)) {
                    cout << RED << "Invalid year!\n" << RESET;
                } else {
                    b.year = year;
                    break;
                }
            }

            file.seekp(-static_cast<int>(sizeof(b)), ios::cur);
            file.write((char*)&b, sizeof(b));
            cout << GREEN << "Book updated successfully.\n" << RESET;
            break;
        }
    }

    if (!found) {
        cout << RED << "Book not found!\n" << RESET;
    }
    file.close();
}

void searchBooks() {
    cout << CYAN << "\n----- Search Books -----\n" << RESET;
    cout << "1. Search by ID\n2. Search by Title\n3. Search by Author\nChoose an option: ";
    int choice;
    cin >> choice;
    clearInput();

    ifstream file("library.dat", ios::binary);
    Book b;
    bool found = false;

    if (choice == 1) {
        int id;
        cout << "Enter Book ID: ";
        cin >> id;
        clearInput();

        while (file.read((char*)&b, sizeof(b))) {
            if (b.id == id) {
                found = true;
                cout << GREEN << "\nBook found:\n" << RESET;
                cout << YELLOW << "ID: " << b.id << "\nTitle: " << b.title << "\nAuthor: " << b.author << "\nPublisher: " << b.publisher << "\nYear: " << b.year << "\n";
                break;
            }
        }
    } else if (choice == 2 || choice == 3) {
        string query;
        cout << "Enter keyword: ";
        getline(cin, query);
        string lowered = toLower(query);

        while (file.read((char*)&b, sizeof(b))) {
            string field = choice == 2 ? b.title : b.author;
            if (toLower(field).find(lowered) != string::npos) {
                found = true;
                cout << GREEN << "\nBook found:\n" << RESET;
                cout << YELLOW << "ID: " << b.id << "\nTitle: " << b.title << "\nAuthor: " << b.author << "\nPublisher: " << b.publisher << "\nYear: " << b.year << "\n";
            }
        }
    }

    if (!found) cout << RED << "No matching records found.\n" << RESET;
    file.close();
}

int main() {
    int choice;
    do {
        cout << BLUE << "\n======= LIBRARY MANAGEMENT SYSTEM =======\n" << RESET;
        cout << GREEN << "1. Add Book\n"<< RESET;
        cout << YELLOW << "2. View Books\n"<< RESET;
        cout << RED << "3. Delete Book\n"<< RESET;
        cout << MAGENTA << "4. Update Book\n"<< RESET;
        cout << CYAN << "5. Exit\n"<< RESET;
        cout << BLUE << "=====================================\n"<< RESET;
        cout << "Choose an option: ";

        cin >> choice;
        clearInput();

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: deleteBook(); break;
            case 4: updateBook(); break;
            case 5: searchBooks(); break;
            case 6: cout << MAGENTA << "Exiting...\n" << RESET; break;
            default: cout << RED << "Invalid option.\n" << RESET;
        }
    } while (choice != 6);

    return 0;
}