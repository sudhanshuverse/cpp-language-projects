#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>  // for std::transform
#include <limits>     // for std::numeric_limits

using namespace std;

// ANSI color codes
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string MAGENTA = "\033[35m";
const string BOLD = "\033[1m";

class Contact {
public:
    string name;
    string phone;
    string email;

    void input() {
        cout << CYAN << "Enter Name: " << RESET;
        getline(cin, name);
        while(name.empty()) {
            cout << RED << "Name cannot be empty. Enter Name: " << RESET;
            getline(cin, name);
        }

        cout << CYAN << "Enter Phone (digits only): " << RESET;
        getline(cin, phone);
        while(!isValidPhone(phone)) {
            cout << RED << "Invalid phone number! Enter digits only: " << RESET;
            getline(cin, phone);
        }

        cout << CYAN << "Enter Email: " << RESET;
        getline(cin, email);
        // Email validation can be added if needed
    }

    void display() const {
        cout << GREEN << BOLD << "Name : " << RESET << name << "\n"
            << GREEN << BOLD << "Phone: " << RESET << phone << "\n"
            << GREEN << BOLD << "Email: " << RESET << email << "\n"
            << YELLOW << "-------------------------\n" << RESET;
    }

    static bool isValidPhone(const string& phone) {
        if(phone.empty()) return false;
        for(char ch : phone) {
            if(!isdigit(ch)) return false;
        }
        return true;
    }
};

vector<Contact> contacts;

// Utility to convert string to lowercase for case-insensitive search
string toLower(const string& s) {
    string res = s;
    transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

// Save contacts vector to file
void saveContacts(const string& filename) {
    ofstream fout(filename, ios::binary);
    if(!fout) {
        cout << RED << "Error saving contacts!" << RESET << "\n";
        return;
    }
    size_t size = contacts.size();
    fout.write((char*)&size, sizeof(size));
    for(const auto& c : contacts) {
        size_t len;

        len = c.name.size();
        fout.write((char*)&len, sizeof(len));
        fout.write(c.name.c_str(), len);

        len = c.phone.size();
        fout.write((char*)&len, sizeof(len));
        fout.write(c.phone.c_str(), len);

        len = c.email.size();
        fout.write((char*)&len, sizeof(len));
        fout.write(c.email.c_str(), len);
    }
    fout.close();
}

// Load contacts from file
void loadContacts(const string& filename) {
    ifstream fin(filename, ios::binary);
    if(!fin) {
        // File doesn't exist yet, no problem
        return;
    }
    size_t size;
    fin.read((char*)&size, sizeof(size));
    contacts.clear();
    for(size_t i = 0; i < size; ++i) {
        Contact c;
        size_t len;

        fin.read((char*)&len, sizeof(len));
        c.name.resize(len);
        fin.read(&c.name[0], len);

        fin.read((char*)&len, sizeof(len));
        c.phone.resize(len);
        fin.read(&c.phone[0], len);

        fin.read((char*)&len, sizeof(len));
        c.email.resize(len);
        fin.read(&c.email[0], len);

        contacts.push_back(c);
    }
    fin.close();
}

// Add new contact
void addContact() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer
    Contact c;
    c.input();
    contacts.push_back(c);
    cout << GREEN << "\nContact added successfully!\n\n" << RESET;
}

// Search contacts by name or phone substring (case-insensitive)
void searchContacts() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << CYAN << "Enter search term (name or phone): " << RESET;
    string term;
    getline(cin, term);
    string lowerTerm = toLower(term);

    bool found = false;
    for (const auto& c : contacts) {
        if(toLower(c.name).find(lowerTerm) != string::npos || c.phone.find(term) != string::npos) {
            c.display();
            found = true;
        }
    }
    if(!found) cout << RED << "No matching contacts found.\n" << RESET;
}

// Update contact by name (first match)
void updateContact() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << CYAN << "Enter name of contact to update: " << RESET;
    string searchName;
    getline(cin, searchName);
    string lowerSearch = toLower(searchName);

    for(auto& c : contacts) {
        if(toLower(c.name) == lowerSearch) {
            cout << GREEN << "Found contact:\n" << RESET;
            c.display();
            cout << CYAN << "Enter new details:\n" << RESET;
            c.input();
            cout << GREEN << "Contact updated!\n" << RESET;
            return;
        }
    }
    cout << RED << "Contact not found!\n" << RESET;
}

// Delete contact by name (first match)
void deleteContact() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << CYAN << "Enter name of contact to delete: " << RESET;
    string searchName;
    getline(cin, searchName);
    string lowerSearch = toLower(searchName);

    auto it = find_if(contacts.begin(), contacts.end(), [&](const Contact& c){
        return toLower(c.name) == lowerSearch;
    });

    if(it != contacts.end()) {
        contacts.erase(it);
        cout << GREEN << "Contact deleted!\n" << RESET;
    } else {
        cout << RED << "Contact not found!\n" << RESET;
    }
}

// Show all contacts
void showAllContacts() {
    if(contacts.empty()) {
        cout << YELLOW << "No contacts to display.\n" << RESET;
        return;
    }
    cout << GREEN << BOLD << "\nAll Contacts:\n-------------------------\n" << RESET;
    for(const auto& c : contacts) {
        c.display();
    }
}

int main() {
    const string filename = "contacts.dat";
    loadContacts(filename);

    int choice;
    do {
        cout << MAGENTA << BOLD << "\n====== Address Book ======\n" << RESET;
        cout << YELLOW << "1. Add Contact\n"
            << "2. Search Contacts\n"
            << "3. Update Contact\n"
            << "4. Delete Contact\n"
            << "5. Show All Contacts\n"
            << "6. Exit\n" << RESET;
        cout << CYAN << "Choose an option: " << RESET;
        cin >> choice;

        switch(choice) {
            case 1: addContact(); break;
            case 2: searchContacts(); break;
            case 3: updateContact(); break;
            case 4: deleteContact(); break;
            case 5: showAllContacts(); break;
            case 6: saveContacts(filename); cout << GREEN << "Exiting... Goodbye!\n" << RESET; break;
            default: cout << RED << "Invalid option! Try again.\n" << RESET;
        }
    } while(choice != 6);

    return 0;
}
