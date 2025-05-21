#include <iostream>
#include <cmath>   // For pow, sqrt, cbrt
#include <limits>  // For numeric_limits
#include <cstdlib> // For system("clear") or system("cls")
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ANSI color codes (text only)
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string MAGENTA = "\033[35m";
const string BOLD = "\033[1m";

// Clear screen cross-platform
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Input validation for double
double getValidNumber(const string &prompt)
{
    double number;
    while (true)
    {
        cout << CYAN << prompt << RESET << "\n> ";
        cin >> number;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << BOLD << "Invalid input! Please enter a valid number." << RESET << "\n";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return number;
        }
    }
}

// Input validation for integer (for factorial, power exponent)
int getValidInt(const string &prompt)
{
    int number;
    while (true)
    {
        cout << CYAN << prompt << RESET << "\n> ";
        cin >> number;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << BOLD << "Invalid input! Please enter a valid integer." << RESET << "\n";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return number;
        }
    }
}

// Factorial function (iterative)
unsigned long long factorial(int n)
{
    if (n < 0)
        return 0; // Invalid factorial
    if (n == 0 || n == 1)
        return 1;
    unsigned long long fact = 1;
    for (int i = 2; i <= n; ++i)
        fact *= i;
    return fact;
}

// Show menu with colorful text
void showMenu()
{
    cout << MAGENTA << BOLD << "==== Advanced Simple Calculator ====" << RESET << "\n";
    cout << CYAN << " 1. Addition (+)" << RESET << "\n";
    cout << CYAN << " 2. Subtraction (-)" << RESET << "\n";
    cout << CYAN << " 3. Multiplication (*)" << RESET << "\n";
    cout << CYAN << " 4. Division (/)" << RESET << "\n";
    cout << CYAN << " 5. Power (x^y)" << RESET << "\n";
    cout << CYAN << " 6. Square (x^2)" << RESET << "\n";
    cout << CYAN << " 7. Cube (x^3)" << RESET << "\n";
    cout << CYAN << " 8. Square Root" << RESET << "\n";
    cout << CYAN << " 9. Cube Root" << RESET << "\n";
    cout << CYAN << "10. Factorial (n!)" << RESET << "\n";
    cout << CYAN << "11. Exit" << RESET << "\n\n";
}

int main()
{
    clearScreen();

    char choice = 'y';
    while (choice == 'y' || choice == 'Y')
    {
        showMenu();
        int option = getValidInt(YELLOW + BOLD + "Choose an option (1-11):" + RESET);

        clearScreen();

        switch (option)
        {
        case 1:
        {
            cout << GREEN << BOLD << "Addition (+)" << RESET << "\n";
            double a = getValidNumber("Enter first number:");
            double b = getValidNumber("Enter second number:");
            cout << GREEN << "Result: " << a << " + " << b << " = " << a + b << RESET << "\n";
            break;
        }
        case 2:
        {
            cout << GREEN << BOLD << "Subtraction (-)" << RESET << "\n";
            double a = getValidNumber("Enter first number:");
            double b = getValidNumber("Enter second number:");
            cout << GREEN << "Result: " << a << " - " << b << " = " << a - b << RESET << "\n";
            break;
        }
        case 3:
        {
            cout << GREEN << BOLD << "Multiplication (*)" << RESET << "\n";
            double a = getValidNumber("Enter first number:");
            double b = getValidNumber("Enter second number:");
            cout << GREEN << "Result: " << a << " * " << b << " = " << a * b << RESET << "\n";
            break;
        }
        case 4:
        {
            cout << GREEN << BOLD << "Division (/)" << RESET << "\n";
            double a = getValidNumber("Enter dividend:");
            double b;
            while (true)
            {
                b = getValidNumber("Enter divisor (non-zero):");
                if (b == 0)
                    cout << RED << BOLD << "Division by zero is not allowed!" << RESET << "\n";
                else
                    break;
            }
            cout << GREEN << "Result: " << a << " / " << b << " = " << a / b << RESET << "\n";
            break;
        }
        case 5:
        {
            cout << GREEN << BOLD << "Power (x^y)" << RESET << "\n";
            double base = getValidNumber("Enter base (x):");
            int exponent = getValidInt("Enter exponent (y):");
            double res = pow(base, exponent);
            cout << GREEN << "Result: " << base << "^" << exponent << " = " << res << RESET << "\n";
            break;
        }
        case 6:
        {
            cout << GREEN << BOLD << "Square (x^2)" << RESET << "\n";
            double x = getValidNumber("Enter number:");
            cout << GREEN << "Result: " << x << "^2 = " << x * x << RESET << "\n";
            break;
        }
        case 7:
        {
            cout << GREEN << BOLD << "Cube (x^3)" << RESET << "\n";
            double x = getValidNumber("Enter number:");
            cout << GREEN << "Result: " << x << "^3 = " << x * x * x << RESET << "\n";
            break;
        }
        case 8:
        {
            cout << GREEN << BOLD << "Square Root (√x)" << RESET << "\n";
            double x;
            while (true)
            {
                x = getValidNumber("Enter number (≥ 0):");
                if (x < 0)
                    cout << RED << BOLD << "Square root of negative number is not allowed!" << RESET << "\n";
                else
                    break;
            }
            cout << GREEN << "Result: √" << x << " = " << sqrt(x) << RESET << "\n";
            break;
        }
        case 9:
        {
            cout << GREEN << BOLD << "Cube Root (∛x)" << RESET << "\n";
            double x = getValidNumber("Enter number:");
            cout << GREEN << "Result: ∛" << x << " = " << cbrt(x) << RESET << "\n";
            break;
        }
        case 10:
        {
            cout << GREEN << BOLD << "Factorial (n!)" << RESET << "\n";
            int n;
            while (true)
            {
                n = getValidInt("Enter a non-negative integer:");
                if (n < 0)
                    cout << RED << BOLD << "Factorial of negative number is undefined!" << RESET << "\n";
                else
                    break;
            }
            unsigned long long fact = factorial(n);
            cout << GREEN << "Result: " << n << "! = " << fact << RESET << "\n";
            break;
        }
        case 11:
        {
            cout << MAGENTA << CYAN << BOLD << "Thank you for using the calculator! Goodbye! 👋" << RESET << "\n";
            return 0;
        }
        default:
            cout << RED << BOLD << "Invalid option. Please choose between 1 and 11." << RESET << "\n";
        }

        cout << CYAN << "Do you want to perform another calculation? (y/n): " << RESET;
        char cont;
        cin >> cont;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        clearScreen();

        if (cont != 'y' && cont != 'Y')
        {
            cout << MAGENTA << CYAN << BOLD << "Thank you for using the calculator! Goodbye!" << RESET << "\n";
            break;
        }
    }

    return 0;
}
