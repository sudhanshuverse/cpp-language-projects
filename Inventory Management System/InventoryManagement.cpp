#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

// Color codes for console output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

struct Item {
    int id;
    char name[50];
    int quantity;
    float price;
};

// Utility function to get a valid integer input from the user
int getValidInt(const string& prompt) {
    int value;
    while (true) {
        cout << CYAN << prompt << RESET;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
            return value;
        } else {
            cout << RED << "Invalid input! Please enter an integer value.\n" << RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Utility function to get a valid float input from the user
float getValidFloat(const string& prompt) {
    float value;
    while (true) {
        cout << CYAN << prompt << RESET;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
            return value;
        } else {
            cout << RED << "Invalid input! Please enter a numeric value.\n" << RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void pause() {
    cout << YELLOW << "\nPress enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void addItem() {
    ofstream fout("inventory.dat", ios::binary | ios::app);
    Item item;

    cout << CYAN << BOLD << "\nEnter Item Details:\n" << RESET;

    item.id = getValidInt("Item ID: ");

    cout << BLUE << "Item Name: " << RESET;
    cin.getline(item.name, 50);

    item.quantity = getValidInt("Quantity: ");

    item.price = getValidFloat("Price: ");

    fout.write(reinterpret_cast<char*>(&item), sizeof(item));
    fout.close();

    cout << GREEN << BOLD << "\nItem added successfully!" << RESET << endl;
    pause();
}

void displayItems() {
    ifstream fin("inventory.dat", ios::binary);
    if (!fin) {
        cout << RED << BOLD << "\nNo records found!" << RESET << endl;
        pause();
        return;
    }

    Item item;

    cout << MAGENTA << BOLD << "\n===== Inventory List =====\n" << RESET;
    cout << setw(10) << "ID" << setw(25) << "Name" << setw(15) << "Quantity" << setw(15) << "Price" << endl;
    cout << string(65, '-') << endl;

    while (fin.read(reinterpret_cast<char*>(&item), sizeof(item))) {
        cout << setw(10) << item.id
             << setw(25) << item.name
             << setw(15) << item.quantity
             << setw(15) << fixed << setprecision(2) << item.price << endl;
    }
    fin.close();
    pause();
}

void searchItem() {
    ifstream fin("inventory.dat", ios::binary);
    if (!fin) {
        cout << RED << BOLD << "\nNo records found!" << RESET << endl;
        pause();
        return;
    }

    int searchId = getValidInt("\nEnter Item ID to search: ");

    Item item;
    bool found = false;

    while (fin.read(reinterpret_cast<char*>(&item), sizeof(item))) {
        if (item.id == searchId) {
            cout << GREEN << BOLD << "\nItem found:\n" << RESET;
            cout << BLUE << "ID: " << RESET << item.id << endl;
            cout << BLUE << "Name: " << RESET << item.name << endl;
            cout << BLUE << "Quantity: " << RESET << item.quantity << endl;
            cout << BLUE << "Price: " << RESET << fixed << setprecision(2) << item.price << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << RED << BOLD << "\nItem not found." << RESET << endl;
    }
    fin.close();
    pause();
}

void deleteItem() {
    ifstream fin("inventory.dat", ios::binary);
    if (!fin) {
        cout << RED << BOLD << "\nNo records found!" << RESET << endl;
        pause();
        return;
    }

    int delId = getValidInt("\nEnter Item ID to delete: ");

    ofstream fout("temp.dat", ios::binary);
    Item item;
    bool found = false;

    while (fin.read(reinterpret_cast<char*>(&item), sizeof(item))) {
        if (item.id == delId) {
            found = true;
            continue;  // skip writing this record
        }
        fout.write(reinterpret_cast<char*>(&item), sizeof(item));
    }

    fin.close();
    fout.close();

    if (found) {
        remove("inventory.dat");
        rename("temp.dat", "inventory.dat");
        cout << GREEN << BOLD << "\nItem deleted successfully!" << RESET << endl;
    } else {
        remove("temp.dat");
        cout << RED << BOLD << "\nItem not found!" << RESET << endl;
    }
    pause();
}

void updateItem() {
    fstream file("inventory.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << RED << BOLD << "\nNo records found!" << RESET << endl;
        pause();
        return;
    }

    int updId = getValidInt("\nEnter Item ID to update: ");

    Item item;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&item), sizeof(item))) {
        if (item.id == updId) {
            cout << MAGENTA << BOLD << "\nCurrent details:\n" << RESET;
            cout << BLUE << "Name: " << RESET << item.name << endl;
            cout << BLUE << "Quantity: " << RESET << item.quantity << endl;
            cout << BLUE << "Price: " << RESET << fixed << setprecision(2) << item.price << endl;

            cout << CYAN << "\nEnter new Name: " << RESET;
            cin.getline(item.name, 50);

            item.quantity = getValidInt("Enter new Quantity: ");

            item.price = getValidFloat("Enter new Price: ");

            int pos = (int)file.tellg() - sizeof(item);
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&item), sizeof(item));

            cout << GREEN << BOLD << "\nItem updated successfully!" << RESET << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << RED << BOLD << "\nItem not found!" << RESET << endl;
    }
    file.close();
    pause();
}

int main() {
    int choice;
    do {
        system("clear");  // Use "cls" on Windows
        cout << BOLD << MAGENTA << "\n=== Inventory Management System ===\n" << RESET;
        cout << YELLOW << "1." << RESET << " Add Item\n";
        cout << YELLOW << "2." << RESET << " Display All Items\n";
        cout << YELLOW << "3." << RESET << " Search Item\n";
        cout << YELLOW << "4." << RESET << " Delete Item\n";
        cout << YELLOW << "5." << RESET << " Update Item\n";
        cout << YELLOW << "6." << RESET << " Exit\n";
        choice = getValidInt("Enter your choice: ");

        switch (choice) {
            case 1: addItem(); break;
            case 2: displayItems(); break;
            case 3: searchItem(); break;
            case 4: deleteItem(); break;
            case 5: updateItem(); break;
            case 6:
                cout << GREEN << BOLD << "\nExiting... Goodbye!\n" << RESET;
                break;
            default:
                cout << RED << BOLD << "\nInvalid choice! Please try again." << RESET << endl;
                pause();
                break;
        }
    } while (choice != 6);

    return 0;
}
