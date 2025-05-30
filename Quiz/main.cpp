#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <conio.h>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <windows.h> // For color text

using namespace std;

// Color codes
enum Color
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    BRIGHT_BLUE = 9,
    BRIGHT_GREEN = 10,
    BRIGHT_CYAN = 11,
    BRIGHT_RED = 12,
    BRIGHT_MAGENTA = 13,
    BRIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};

// Function to set text color
void setColor(Color textColor, Color bgColor = BLACK)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

// User structure to store user information
struct User
{
    string username;
    string password;
    string name;
};

// Question structure
struct Question
{
    string subject;
    string text;
    string options[4];
    char correctAnswer;
};

// Function prototypes
void displayWelcomeScreen();
void login();
void signup();
bool validateUsername(const string &username);
bool validatePassword(const string &password);
void mainMenu(const User &user);
void selectSubject(const User &user);
void takeQuiz(const User &user, const string &subject);
vector<Question> loadQuestions(const string &subject);
void displayQuestion(const Question &q, int questionNumber);
void clearScreen();
string toLower(const string &str);
void viewResults(const User &user);

// Global variables
const string QUESTIONS_FILE = "questions.txt";
const string USERS_FILE = "users.txt";

int main()
{
    srand(time(0)); // Seed for random number generation
    displayWelcomeScreen();
    return 0;
}

void displayWelcomeScreen()
{
    clearScreen();
    setColor(BRIGHT_CYAN);
    cout << "========================================\n";
    setColor(BRIGHT_YELLOW);
    cout << "       WELCOME TO MCQ QUIZ SYSTEM      \n";
    setColor(BRIGHT_CYAN);
    cout << "========================================\n\n";

    setColor(BRIGHT_WHITE);
    cout << "1. ";
    setColor(BRIGHT_GREEN);
    cout << "Login\n";

    setColor(BRIGHT_WHITE);
    cout << "2. ";
    setColor(BRIGHT_GREEN);
    cout << "Sign Up\n";

    setColor(BRIGHT_WHITE);
    cout << "3. ";
    setColor(BRIGHT_RED);
    cout << "Exit\n";

    setColor(BRIGHT_WHITE);
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;
    cin.ignore(); // Clear input buffer

    switch (choice)
    {
    case 1:
        login();
        break;
    case 2:
        signup();
        break;
    case 3:
        exit(0);
    default:
        setColor(BRIGHT_RED);
        cout << "Invalid choice. Please try again.\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        displayWelcomeScreen();
    }
}

void login()
{
    clearScreen();
    setColor(BRIGHT_CYAN);
    cout << "========== LOGIN ==========\n\n";

    string username, password;
    setColor(BRIGHT_WHITE);
    cout << "Username: ";
    setColor(BRIGHT_YELLOW);
    getline(cin, username);

    setColor(BRIGHT_WHITE);
    cout << "Password: ";
    // Password masking
    char ch;
    password = "";
    while ((ch = _getch()) != '\r')
    { // '\r' is Enter key
        if (ch == '\b')
        { // Handle backspace
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            password.push_back(ch);
            setColor(BRIGHT_MAGENTA);
            cout << '*';
        }
    }
    cout << endl;

    ifstream file(USERS_FILE);
    string line;
    bool found = false;
    User user;

    while (getline(file, line))
    {
        size_t comma1 = line.find(',');
        size_t comma2 = line.find(',', comma1 + 1);

        if (comma1 != string::npos && comma2 != string::npos)
        {
            string fileUsername = line.substr(0, comma1);
            string filePassword = line.substr(comma1 + 1, comma2 - comma1 - 1);

            if (fileUsername == username && filePassword == password)
            {
                found = true;
                user.username = fileUsername;
                user.password = filePassword;
                user.name = line.substr(comma2 + 1);
                break;
            }
        }
    }
    file.close();

    if (found)
    {
        setColor(BRIGHT_GREEN);
        cout << "\nLogin successful! Welcome, ";
        setColor(BRIGHT_YELLOW);
        cout << user.name << "!\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        mainMenu(user);
    }
    else
    {
        setColor(BRIGHT_RED);
        cout << "\nInvalid username or password. Please try again.\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        displayWelcomeScreen();
    }
}

void signup()
{
    clearScreen();
    setColor(BRIGHT_CYAN);
    cout << "========== SIGN UP ==========\n\n";

    User newUser;
    setColor(BRIGHT_WHITE);
    cout << "Enter your full name: ";
    setColor(BRIGHT_YELLOW);
    getline(cin, newUser.name);

    bool validUsername = false;
    while (!validUsername)
    {
        setColor(BRIGHT_WHITE);
        cout << "Choose a username (alphanumeric, 5-15 chars): ";
        setColor(BRIGHT_YELLOW);
        getline(cin, newUser.username);

        validUsername = validateUsername(newUser.username);
        if (!validUsername)
        {
            setColor(BRIGHT_RED);
            cout << "Invalid username. Please follow the rules:\n";
            setColor(BRIGHT_WHITE);
            cout << "- 5 to 15 characters\n";
            cout << "- Only letters and numbers\n";
            cout << "- No spaces or special characters\n";
        }
        else
        {
            // Check if username exists
            ifstream file(USERS_FILE);
            string line;
            bool exists = false;

            while (getline(file, line))
            {
                size_t comma = line.find(',');
                if (comma != string::npos)
                {
                    string fileUsername = line.substr(0, comma);
                    if (fileUsername == newUser.username)
                    {
                        exists = true;
                        break;
                    }
                }
            }
            file.close();

            if (exists)
            {
                setColor(BRIGHT_RED);
                cout << "Username already exists. Please choose another.\n";
                validUsername = false;
            }
        }
    }

    bool validPassword = false;
    while (!validPassword)
    {
        setColor(BRIGHT_WHITE);
        cout << "Choose a password (8-20 chars, at least 1 digit and 1 special char): ";

        // Password masking
        char ch;
        newUser.password = "";
        while ((ch = _getch()) != '\r')
        {
            if (ch == '\b')
            {
                if (!newUser.password.empty())
                {
                    newUser.password.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                newUser.password.push_back(ch);
                setColor(BRIGHT_MAGENTA);
                cout << '*';
            }
        }
        cout << endl;

        validPassword = validatePassword(newUser.password);
        if (!validPassword)
        {
            setColor(BRIGHT_RED);
            cout << "Invalid password. Please follow the rules:\n";
            setColor(BRIGHT_WHITE);
            cout << "- 8 to 20 characters\n";
            cout << "- At least one digit\n";
            cout << "- At least one special character (!@#$%^&*)\n";
        }
    }

    // Save user to file
    ofstream file(USERS_FILE, ios::app);
    if (file.is_open())
    {
        file << newUser.username << "," << newUser.password << "," << newUser.name << endl;
        file.close();

        setColor(BRIGHT_GREEN);
        cout << "\nRegistration successful! You can now login with your credentials.\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        displayWelcomeScreen();
    }
    else
    {
        setColor(BRIGHT_RED);
        cout << "Error saving user data. Please try again.\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        signup();
    }
}

// [Rest of the functions remain the same as before, just add color to their outputs]

void clearScreen()
{
    system("cls");
}

string toLower(const string &str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// [Previous includes and definitions remain the same...]

bool validateUsername(const string &username)
{
    if (username.length() < 5 || username.length() > 15)
        return false;
    return all_of(username.begin(), username.end(), [](char c)
                  { return isalnum(c); });
}

bool validatePassword(const string &password)
{
    if (password.length() < 8 || password.length() > 20)
        return false;

    bool hasDigit = false;
    bool hasSpecial = false;
    const string specialChars = "!@#$%^&*";

    for (char c : password)
    {
        if (isdigit(c))
            hasDigit = true;
        if (specialChars.find(c) != string::npos)
            hasSpecial = true;
    }

    return hasDigit && hasSpecial;
}

void mainMenu(const User &user)
{
    clearScreen();
    setColor(BRIGHT_CYAN);
    cout << "===============================================\n";
    setColor(BRIGHT_YELLOW);
    cout << "          MAIN MENU - WELCOME " << user.name << "\n";
    setColor(BRIGHT_CYAN);
    cout << "===============================================\n\n";

    setColor(BRIGHT_WHITE);
    cout << "1. ";
    setColor(BRIGHT_GREEN);
    cout << "Take a Quiz\n";

    setColor(BRIGHT_WHITE);
    cout << "2. ";
    setColor(BRIGHT_BLUE);
    cout << "View Previous Results\n";

    setColor(BRIGHT_WHITE);
    cout << "3. ";
    setColor(BRIGHT_MAGENTA);
    cout << "Change Password\n";

    setColor(BRIGHT_WHITE);
    cout << "4. ";
    setColor(BRIGHT_RED);
    cout << "Logout\n";

    setColor(BRIGHT_WHITE);
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        selectSubject(user);
        break;
    case 2:
        viewResults(user);
        break;
    case 3:
        // Implement password change functionality
        break;
    case 4:
        displayWelcomeScreen();
        break;
    default:
        setColor(BRIGHT_RED);
        cout << "Invalid choice. Please try again.\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        mainMenu(user);
    }
}

void selectSubject(const User &user)
{
    clearScreen();
    setColor(BRIGHT_CYAN);
    cout << "========================================\n";
    setColor(BRIGHT_YELLOW);
    cout << "       SELECT QUIZ SUBJECT\n";
    setColor(BRIGHT_CYAN);
    cout << "========================================\n\n";

    vector<string> subjects = {
        "C", "C++", "HTML", "CSS",
        "JavaScript", "PHP", "Python", "Java"};

    for (int i = 0; i < subjects.size(); i++)
    {
        setColor(BRIGHT_WHITE);
        cout << i + 1 << ". ";
        setColor(BRIGHT_GREEN);
        cout << subjects[i] << "\n";
    }

    setColor(BRIGHT_WHITE);
    cout << subjects.size() + 1 << ". ";
    setColor(BRIGHT_RED);
    cout << "Back to Main Menu\n\n";

    setColor(BRIGHT_WHITE);
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    if (choice > 0 && choice <= subjects.size())
    {
        takeQuiz(user, subjects[choice - 1]);
    }
    else if (choice == subjects.size() + 1)
    {
        mainMenu(user);
    }
    else
    {
        setColor(BRIGHT_RED);
        cout << "Invalid choice. Please try again.\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        selectSubject(user);
    }
}

void takeQuiz(const User &user, const string &subject)
{
    clearScreen();
    vector<Question> questions = loadQuestions(subject);

    if (questions.empty())
    {
        setColor(BRIGHT_RED);
        cout << "No questions available for " << subject << " yet.\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        selectSubject(user);
        return;
    }

    // Shuffle questions
    random_shuffle(questions.begin(), questions.end());

    int score = 0;
    const int totalQuestions = min(10, (int)questions.size()); // Limit to 10 questions

    setColor(BRIGHT_CYAN);
    cout << "========================================\n";
    setColor(BRIGHT_YELLOW);
    cout << "       " << subject << " QUIZ - " << totalQuestions << " QUESTIONS\n";
    setColor(BRIGHT_CYAN);
    cout << "========================================\n\n";

    for (int i = 0; i < totalQuestions; i++)
    {
        displayQuestion(questions[i], i + 1);

        setColor(BRIGHT_WHITE);
        cout << "Your answer (A/B/C/D): ";
        char answer;
        cin >> answer;
        cin.ignore();
        answer = toupper(answer);

        if (answer == questions[i].correctAnswer)
        {
            setColor(BRIGHT_GREEN);
            cout << "Correct!\n";
            score++;
        }
        else
        {
            setColor(BRIGHT_RED);
            cout << "Incorrect! The correct answer was " << questions[i].correctAnswer << "\n";
        }
        setColor(BRIGHT_WHITE);
        cout << "----------------------------------------\n";
    }

    // Display results
    setColor(BRIGHT_CYAN);
    cout << "\n========================================\n";
    setColor(BRIGHT_YELLOW);
    cout << "             QUIZ RESULTS\n";
    setColor(BRIGHT_CYAN);
    cout << "========================================\n\n";

    setColor(BRIGHT_WHITE);
    cout << "Subject: ";
    setColor(BRIGHT_YELLOW);
    cout << subject << "\n";

    setColor(BRIGHT_WHITE);
    cout << "Questions Attempted: ";
    setColor(BRIGHT_YELLOW);
    cout << totalQuestions << "\n";

    setColor(BRIGHT_WHITE);
    cout << "Correct Answers: ";
    if (score == totalQuestions)
    {
        setColor(BRIGHT_GREEN);
    }
    else if (score >= totalQuestions / 2)
    {
        setColor(BRIGHT_YELLOW);
    }
    else
    {
        setColor(BRIGHT_RED);
    }
    cout << score << "\n";

    setColor(BRIGHT_WHITE);
    cout << "Your Score: ";
    if (score == totalQuestions)
    {
        setColor(BRIGHT_GREEN);
    }
    else if (score >= totalQuestions / 2)
    {
        setColor(BRIGHT_YELLOW);
    }
    else
    {
        setColor(BRIGHT_RED);
    }
    cout << (score * 100 / totalQuestions) << "%\n\n";

    // Save results to file
    ofstream resultFile("results_" + user.username + ".txt", ios::app);
    if (resultFile.is_open())
    {
        time_t now = time(0);
        char *dt = ctime(&now);
        resultFile << "Date: " << dt;
        resultFile << "Subject: " << subject << "\n";
        resultFile << "Score: " << score << "/" << totalQuestions << "\n";
        resultFile << "Percentage: " << (score * 100 / totalQuestions) << "%\n";
        resultFile << "----------------------------------------\n";
        resultFile.close();
    }

    setColor(BRIGHT_WHITE);
    system("pause");
    selectSubject(user);
}

vector<Question> loadQuestions(const string &subject)
{
    vector<Question> questions;
    ifstream file(QUESTIONS_FILE);
    string line;
    Question currentQuestion;
    bool readingQuestion = false;
    int optionCount = 0;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '[' && line[line.length() - 1] == ']')
        {
            string questionSubject = line.substr(1, line.length() - 2);
            if (toLower(questionSubject) == toLower(subject))
            {
                readingQuestion = true;
                currentQuestion.subject = questionSubject;
                optionCount = 0;
            }
            else
            {
                readingQuestion = false;
            }
        }
        else if (readingQuestion)
        {
            if (optionCount == 0)
            {
                currentQuestion.text = line;
                optionCount++;
            }
            else if (optionCount < 5)
            {
                if (optionCount < 5)
                {
                    currentQuestion.options[optionCount - 1] = line;
                    optionCount++;
                }
            }
            else
            {
                currentQuestion.correctAnswer = toupper(line[0]);
                questions.push_back(currentQuestion);
                readingQuestion = false;
            }
        }
    }
    file.close();

    return questions;
}

void displayQuestion(const Question &q, int questionNumber)
{
    setColor(BRIGHT_WHITE);
    cout << "Question " << questionNumber << ": ";
    setColor(BRIGHT_YELLOW);
    cout << q.text << "\n\n";

    setColor(BRIGHT_WHITE);
    cout << "A. ";
    setColor(BRIGHT_BLUE);
    cout << q.options[0] << "\n";

    setColor(BRIGHT_WHITE);
    cout << "B. ";
    setColor(BRIGHT_BLUE);
    cout << q.options[1] << "\n";

    setColor(BRIGHT_WHITE);
    cout << "C. ";
    setColor(BRIGHT_BLUE);
    cout << q.options[2] << "\n";

    setColor(BRIGHT_WHITE);
    cout << "D. ";
    setColor(BRIGHT_BLUE);
    cout << q.options[3] << "\n\n";
}

void viewResults(const User &user)
{
    clearScreen();
    setColor(BRIGHT_CYAN);
    cout << "========================================\n";
    setColor(BRIGHT_YELLOW);
    cout << "       YOUR QUIZ RESULTS HISTORY\n";
    setColor(BRIGHT_CYAN);
    cout << "========================================\n\n";

    ifstream resultFile("results_" + user.username + ".txt");
    if (resultFile.is_open())
    {
        string line;
        while (getline(resultFile, line))
        {
            if (line.find("Date:") != string::npos)
            {
                setColor(BRIGHT_WHITE);
                cout << "\n";
            }

            if (line.find("Score:") != string::npos)
            {
                size_t slashPos = line.find('/');
                if (slashPos != string::npos)
                {
                    int score = stoi(line.substr(7, slashPos - 7));
                    int total = stoi(line.substr(slashPos + 1));

                    if (score == total)
                    {
                        setColor(BRIGHT_GREEN);
                    }
                    else if (score >= total / 2)
                    {
                        setColor(BRIGHT_YELLOW);
                    }
                    else
                    {
                        setColor(BRIGHT_RED);
                    }
                }
            }

            cout << line << "\n";
            setColor(BRIGHT_WHITE);
        }
        resultFile.close();
    }
    else
    {
        setColor(BRIGHT_RED);
        cout << "No quiz results found.\n";
    }

    setColor(BRIGHT_WHITE);
    cout << "\nPress any key to return to main menu...";
    _getch();
    mainMenu(user);
}

// [Previous includes and definitions remain the same...]

void changePassword(User &user)
{
    clearScreen();
    setColor(BRIGHT_CYAN);
    cout << "========================================\n";
    setColor(BRIGHT_YELLOW);
    cout << "       CHANGE PASSWORD\n";
    setColor(BRIGHT_CYAN);
    cout << "========================================\n\n";

    string currentPassword, newPassword;
    bool validPassword = false;

    // Verify current password
    setColor(BRIGHT_WHITE);
    cout << "Enter current password: ";
    char ch;
    currentPassword = "";
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (!currentPassword.empty())
            {
                currentPassword.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            currentPassword.push_back(ch);
            setColor(BRIGHT_MAGENTA);
            cout << '*';
        }
    }
    cout << endl;

    if (currentPassword != user.password)
    {
        setColor(BRIGHT_RED);
        cout << "Incorrect current password!\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        mainMenu(user);
        return;
    }

    // Get new password
    while (!validPassword)
    {
        setColor(BRIGHT_WHITE);
        cout << "Enter new password (8-20 chars, at least 1 digit and 1 special char): ";

        newPassword = "";
        while ((ch = _getch()) != '\r')
        {
            if (ch == '\b')
            {
                if (!newPassword.empty())
                {
                    newPassword.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                newPassword.push_back(ch);
                setColor(BRIGHT_MAGENTA);
                cout << '*';
            }
        }
        cout << endl;

        validPassword = validatePassword(newPassword);
        if (!validPassword)
        {
            setColor(BRIGHT_RED);
            cout << "Invalid password. Please follow the rules:\n";
            setColor(BRIGHT_WHITE);
            cout << "- 8 to 20 characters\n";
            cout << "- At least one digit\n";
            cout << "- At least one special character (!@#$%^&*)\n";
        }
    }

    // Confirm new password
    setColor(BRIGHT_WHITE);
    cout << "Confirm new password: ";
    string confirmPassword = "";
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (!confirmPassword.empty())
            {
                confirmPassword.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            confirmPassword.push_back(ch);
            setColor(BRIGHT_MAGENTA);
            cout << '*';
        }
    }
    cout << endl;

    if (newPassword != confirmPassword)
    {
        setColor(BRIGHT_RED);
        cout << "Passwords don't match!\n";
        setColor(BRIGHT_WHITE);
        system("pause");
        changePassword(user);
        return;
    }

    // Update password in users file
    ifstream inFile(USERS_FILE);
    ofstream outFile("temp_users.txt");
    string line;
    bool updated = false;

    while (getline(inFile, line))
    {
        size_t comma1 = line.find(',');
        size_t comma2 = line.find(',', comma1 + 1);

        if (comma1 != string::npos && comma2 != string::npos)
        {
            string fileUsername = line.substr(0, comma1);

            if (fileUsername == user.username)
            {
                outFile << user.username << "," << newPassword << "," << user.name << endl;
                updated = true;
                user.password = newPassword;
            }
            else
            {
                outFile << line << endl;
            }
        }
    }

    inFile.close();
    outFile.close();

    if (updated)
    {
        remove(USERS_FILE.c_str());
        rename("temp_users.txt", USERS_FILE.c_str());

        setColor(BRIGHT_GREEN);
        cout << "\nPassword changed successfully!\n";
    }
    else
    {
        setColor(BRIGHT_RED);
        cout << "\nError updating password. Please try again.\n";
    }

    setColor(BRIGHT_WHITE);
    system("pause");
    mainMenu(user);
}

void showLoading(string message, int dots = 3, int delay = 300)
{
    setColor(BRIGHT_YELLOW);
    cout << message;
    for (int i = 0; i < dots; i++)
    {
        cout << ".";
        cout.flush();
        Sleep(delay);
    }
    cout << "\n";
    setColor(BRIGHT_WHITE);
}

void displayAbout()
{
    clearScreen();
    setColor(BRIGHT_CYAN);
    cout << "========================================\n";
    setColor(BRIGHT_YELLOW);
    cout << "          ABOUT MCQ QUIZ SYSTEM\n";
    setColor(BRIGHT_CYAN);
    cout << "========================================\n\n";

    setColor(BRIGHT_WHITE);
    cout << "Version: 2.0\n";
    cout << "Developed by: Your Name\n";
    cout << "Features:\n";
    setColor(BRIGHT_GREEN);
    cout << "- User authentication (login/signup)\n";
    cout << "- Multiple quiz subjects\n";
    cout << "- Colorful interface\n";
    cout << "- Performance tracking\n";
    cout << "- Secure password storage\n\n";

    setColor(BRIGHT_WHITE);
    cout << "Press any key to return...";
    _getch();
}

void adminPanel()
{
    clearScreen();
    setColor(BRIGHT_CYAN);
    cout << "========================================\n";
    setColor(BRIGHT_YELLOW);
    cout << "          ADMIN PANEL\n";
    setColor(BRIGHT_CYAN);
    cout << "========================================\n\n";

    // Add admin functionality here
    // For example: add questions, view all users, etc.

    setColor(BRIGHT_WHITE);
    cout << "1. Add New Questions\n";
    cout << "2. View All Users\n";
    cout << "3. Back to Main Menu\n\n";

    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        // Implement question addition
        break;
    case 2:
        // Implement user listing
        break;
    case 3:
        return;
    default:
        setColor(BRIGHT_RED);
        cout << "Invalid choice!\n";
        setColor(BRIGHT_WHITE);
        system("pause");
    }

    adminPanel();
}

void addSampleQuestions()
{
    // This function would add sample questions to the questions file
    // Useful for first-time setup
    ofstream qFile(QUESTIONS_FILE, ios::app);
    if (qFile.is_open())
    {
        qFile << "\n[C++]\n";
        qFile << "Which of these is not a C++ data type?\n";
        qFile << "int\n";
        qFile << "float\n";
        qFile << "real\n";
        qFile << "double\n";
        qFile << "C\n";

        qFile << "\n[Python]\n";
        qFile << "What is used to define a block of code in Python?\n";
        qFile << "Curly braces\n";
        qFile << "Indentation\n";
        qFile << "Parentheses\n";
        qFile << "Square brackets\n";
        qFile << "B\n";

        qFile.close();
        setColor(BRIGHT_GREEN);
        cout << "Sample questions added successfully!\n";
    }
    else
    {
        setColor(BRIGHT_RED);
        cout << "Error adding sample questions!\n";
    }
    setColor(BRIGHT_WHITE);
}

void initializeSystem()
{
    // Check if questions file exists, if not create with samples
    ifstream qFile(QUESTIONS_FILE);
    if (!qFile.good())
    {
        addSampleQuestions();
    }
    qFile.close();

    // Check if users file exists, if not create empty
    ofstream uFile(USERS_FILE, ios::app);
    uFile.close();
}