#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

// Namespace
using namespace std;

//====================== Styling Constants ======================//
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string MAGENTA = "\033[35m";
const string BOLD = "\033[1m";

// Console width for center alignment
const int CONSOLE_WIDTH = 80;

//====================== Utility Functions ======================//

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printCentered(const string &text)
{
    int padding = (CONSOLE_WIDTH - static_cast<int>(text.size())) / 2;
    cout << string(max(0, padding), ' ') << text << '\n';
}

//====================== Game Logic ======================//

enum class Player
{
    None = ' ',
    X = 'X',
    O = 'O'
};

void displayBoard(const vector<Player> &board)
{
    clearScreen();
    printCentered(BOLD + CYAN + "===== Tic-Tac-Toe Game =====" + RESET);
    printCentered("");

    for (int i = 0; i < 9; i += 3)
    {
        string row;
        for (int j = i; j < i + 3; ++j)
        {
            string cell;
            if (board[j] == Player::X)
                cell = RED + "X" + RESET;
            else if (board[j] == Player::O)
                cell = GREEN + "O" + RESET;
            else
                cell = YELLOW + to_string(j + 1) + RESET;

            row += " " + cell + (j < i + 2 ? " |" : "");
        }
        printCentered(row);
        if (i < 6)
            printCentered("---+---+---");
    }
    printCentered("");
}

bool checkWin(const vector<Player> &board, Player current)
{
    const int winCombos[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

    for (const auto &combo : winCombos)
    {
        if (board[combo[0]] == current &&
            board[combo[1]] == current &&
            board[combo[2]] == current)
            return true;
    }
    return false;
}

bool isDraw(const vector<Player> &board)
{
    for (const auto &cell : board)
        if (cell == Player::None)
            return false;
    return true;
}

Player switchPlayer(Player current)
{
    return (current == Player::X) ? Player::O : Player::X;
}

int getValidMove(const vector<Player> &board, Player current)
{
    int move;
    while (true)
    {
        printCentered(BOLD + MAGENTA + "Player " + string(1, static_cast<char>(current)) + ", enter position (1-9): " + RESET);
        cin >> move;

        if (cin.fail() || move < 1 || move > 9 || board[move - 1] != Player::None)
        {
            cin.clear();
            cin.ignore(10000, '\n');
            printCentered(RED + "Invalid input. Please try again." + RESET);
        }
        else
        {
            return move - 1;
        }
    }
}

//====================== Main ======================//

int main()
{
    vector<Player> board(9, Player::None);
    Player currentPlayer = Player::X;
    bool gameOver = false;

    while (!gameOver)
    {
        displayBoard(board);
        int move = getValidMove(board, currentPlayer);
        board[move] = currentPlayer;

        if (checkWin(board, currentPlayer))
        {
            displayBoard(board);
            printCentered(GREEN + BOLD + "Player " + string(1, static_cast<char>(currentPlayer)) + " wins!" + RESET);
            gameOver = true;
        }
        else if (isDraw(board))
        {
            displayBoard(board);
            printCentered(YELLOW + BOLD + "It's a draw!" + RESET);
            gameOver = true;
        }
        else
        {
            currentPlayer = switchPlayer(currentPlayer);
        }
    }

    printCentered(CYAN + "Thanks for playing! Press Enter to exit." + RESET);
    cin.ignore();
    cin.get();
    return 0;
}
