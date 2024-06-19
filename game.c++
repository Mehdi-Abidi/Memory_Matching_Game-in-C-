#include <iostream>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <iomanip>
#define V 25
#define H 79
using namespace std;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD position;

void coordinates(int x, int y) 
{
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(console, position);
}
class Node
{
public:
    int value;
    bool flipped;
    Node* next;
    Node(int val) :value(val), flipped(false), next(nullptr) {}
};

void gameborder() //game border
{
    void coordinates(int x, int y);
    system("cls");

    for (int i = 2; i <= H; i++) //horizontal lines
    {
        coordinates(i, 1);
        cout << '-';
        coordinates(i, V);
        cout << '-';
    }

    for (int i = 1; i <= V; i++) //vertical lines
    {
        coordinates(2, i);
        cout << '|';
        coordinates(H, i);
        cout << '|';
    }

    for (int i = 3; i <= H - 1; i++) //line below header
    {
        coordinates(i, 4);
        cout << '-';

    }
}

class Score {
public:
    int max;
    int top;
    int* stack;
    int* rounds;
    int highs = 0;
    int top2;
    int r = 1;

    Score(int size) {
        max = size;
        stack = new int[max];
        rounds = new int[max];
        top = -1;
        top2 = -1;

    }

    ~Score() {
        delete[] stack;
    }

    void Push(int data) {
        stack[++top] = data;
        rounds[++top2] = r;
        r++;
    }

    int hscore() {
        highs = 0;
        for (int i = 0; i <= top; ++i) {
            if (stack[i] > highs) {
                highs = stack[i];
            }
        }
        return highs;
    }


    int Top() {
        return stack[top];
    }

    void swap(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

    void quicksort(int low, int high) {
        if (low < high) {
            int pivotIndex = partition(low, high);
            quicksort(low, pivotIndex - 1);
            quicksort(pivotIndex + 1, high);
        }
    }

    int partition(int low, int high) {
        int pivot = stack[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (stack[j] >= pivot) {
                i++;
                swap(stack[i], stack[j]);
                swap(rounds[i], rounds[j]);
            }
        }

        swap(stack[i + 1], stack[high]);
        swap(rounds[i + 1], rounds[high]);
        return i + 1;
    }

    void sortScores() {
        quicksort(0, top);
    }
};

class Game {
private:
    int size;
    Node* head;
    int attempts;
    int rounds;
    int score;
    int static cround;
    int tattempts;
    int ch = 1;
    char smile = ch;
public:
    Score s;

    Game(int r) : head(nullptr), attempts(0), rounds(r), s(r) {}

    void initialize(int s, int a) {
        size = s;
        tattempts = a;
        int* cards = new int[size * size];
        for (int i = 0; i < (size * size) - 1; i++) {
            cards[i] = i + 1;
            cards[i + 1] = i + 1;
            i++;
        }
        srand(static_cast<unsigned int>(time(0)));
        for (int i = (size * size) - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            swap(cards[i], cards[j]);
        }

        for (int i = 0; i < (size * size); ++i) {
            Node* newNode = new Node(cards[i]);
            newNode->next = head;
            head = newNode;
        }
        score = 0;
        attempts = 0;
    }

    void displayBoardValues() {
        HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(col, 12);
        Node* current = head;

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cout << setw(4);
                cout << current->value;
                current = current->next;
            }
            coordinates(6, i + 8);
        }
    }

    void displayBoard() {
        Node* current = head;
        HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(col, 12);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cout << setw(4);
                if (current->flipped) {
                    HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(col, 11);
                    cout << current->value;
                }
                else {
                    HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(col, 12);
                    cout << (i * size + j);
                }
                current = current->next;
            }
            coordinates(6, i + 8);
        }
    }


    Node* findNode(Node* current, int position) {
        if (position == 0 || current == nullptr) {
            return current;
        }
        return findNode(current->next, position - 1);
    }

    bool isGameOver() {
        Node* current = head;
        while (current != nullptr) {
            if (!current->flipped) {
                return false;
            }
            current = current->next;
        }
        return true;
    }

    void revealCard(Node* current, int position) {

        if (position == 0) {
            current->flipped = true;
            return;
        }
        revealCard(current->next, position - 1);
    }

    void playMemoryGame() {
        while (!isGameOver()&&attempts<tattempts) {
            system("cls");
            gameborder();
            HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(col, 14);
            coordinates(4, 3);
            cout << "ATTEMPTS: "<<attempts;
            coordinates(44, 3);
            cout << "HIGHSCORE: " << s.hscore();
            coordinates(24, 3);
            cout << "ROUND: " << cround;
            coordinates(6, 6);
            displayBoard();

            int pos1, pos2;
            SetConsoleTextAttribute(col, 14);
            coordinates(6, 12);
            cout << "Enter the position of the first card: ";
            cin >> pos1;
            coordinates(6, 13);
            cout << "Enter the position of the second card: ";
            cin >> pos2;
            coordinates(6, 14);
            if (pos1 < 0 || pos1 >= size * size || pos2 < 0 || pos2 >= size * size ||
                findNode(head, pos1) == nullptr || findNode(head, pos2) == nullptr ||
                findNode(head, pos1)->flipped || findNode(head, pos2)->flipped || pos1 == pos2) {
                SetConsoleTextAttribute(col, 14);
                cout << "Invalid input. Try again."; coordinates(6, 15);
                continue;
            }

            if (findNode(head, pos1)->value == findNode(head, pos2)->value) {
                SetConsoleTextAttribute(col, 14);
                cout << "Match found!"; coordinates(6,16);
                score++;
                revealCard(head, pos1);
                revealCard(head, pos2);
            }
            else {
                SetConsoleTextAttribute(col, 14);
                cout << "No match. Try again."; coordinates(6, 16);
            }

            attempts++;
            Sleep(2000);
        }


        cround++;
        HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(col, 11);
        if (attempts >= tattempts)
        {
            cout << "You failed to complete this round in the given attempts."; coordinates(6, 17);
            s.Push(0);
        }
        else
        {
            cout << "Congratulations! You completed the game in " << attempts << " attempts." ; coordinates(6, 17);
            s.Push(100 - attempts + score);
        }
        cout << "Score: " << s.Top() << endl;
        Sleep(2000);
        system("cls");
    }

    void Performance()
    {
        int i;
        coordinates(24, 3);
        cout << "GAME SUMMARY";
        s.sortScores();
        for ( i = 0; i < rounds; i++)
        {
            coordinates(6, i + 6);
            cout << "Round#" << s.rounds[i] << "\tScore:" << s.stack[i];
            if (s.stack[i] == 0)
            {
                cout << " (failed)"; coordinates(6, i+6+1);
            }
            else
            {
                coordinates(6, i+6+1);
            }
        }
        coordinates(6, i+6+2);
        cout << "High Score: " << s.hscore() << endl;
        coordinates(6, i+6+3);
        cout << "THANK YOU FOR PLAYING :) ";
    }

    ~Game() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
};
int Game::cround = 1;
int main() {
    coordinates(34, 4);
    cout << "Memory Matching Game";
    coordinates(34, 10);
    cout << "Press any key to play the Game";
    coordinates(34, 12);
    cout << "Instructions: Enter number of rounds and size of board.";
    coordinates(34, 13);
    cout << "Board will be displayed for 3 seconds. Then round begins.";
    coordinates(34, 14);
    cout << "Enter the positions of 2 matching cards.";
    coordinates(34, 15);
    cout << "The more attempts you make, the lower the score.";
    coordinates(34, 16);
    cout << "You have a limited number of attempts.";
    coordinates(34, 20);
    cout << "Made by, Rida (22k-4409), Mashal (22k-4552), Mehdi (22k-4480)";
    while (!_kbhit()) {
    }
    system("cls");
    HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(col, 14);
    int boardSize;
    int rounds;
    int attempts;
    gameborder();
    coordinates(6, 6);
    cout << "Enter the number of rounds (less than 10): ";
    cin >> rounds;

    while (rounds < 0 || rounds>10) {
        cout << "Invalid rounds size. ";
        cin >> rounds;
    }
    Game game(rounds);
    for (int i = 0; i < rounds; i++)
    {
        gameborder();
        coordinates(6, 7);
        cout << "Enter the size of the board (even number): ";
        cin >> boardSize;
        while (boardSize % 2 != 0 || boardSize < 0) {
            cout << "Invalid board size. Please enter an even number. ";
            cin >> boardSize;
        }
        attempts = boardSize * boardSize + 2;
        game.initialize(boardSize,attempts);
        SetConsoleTextAttribute(col, 14);
        Sleep(1000);
        system("cls");
        gameborder();
        coordinates(6, 6);
        cout << "Showing you the cards! You have " << attempts << " attempts for this round. Good luck!";
        coordinates(6, 7);
        game.displayBoardValues();
        Sleep(3000);
        system("cls");
        gameborder();

        game.playMemoryGame();
        if (i == rounds - 1)
        {
            Sleep(2000);
            system("cls");
            gameborder();
            coordinates(6, 6);
            game.Performance();
        }
    }
    return 0;
}
