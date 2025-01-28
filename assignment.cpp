
/**********|**********|**********|
Program: assignment.cpp
Course: Data Structures and Algorithms
Trimester: 2410
Name: Wisyal Faridz Aimizil bin Mohd Fauzi
ID: 1221304904
Lecture Section: TC1L
Tutorial Section: TT4L
Email: 1221304904@student.mmu.edu.my
Phone: 018-3553588

Name: Abdul Nasser Kolathumkara Muhammed Nazim
ID: 241UC241F9
Lecture Section: TC1L
Tutorial Section: TT4L
Email: abdul.nasser.kolathumkara@student.mmu.edu.my
Phone: 014-9323413

Name: Frank Carrano
ID: 1071001234
Lecture Section: TC101
Tutorial Section: TT1L
Email: abc123@yourmail.com
Phone: 018-1234567

Name: Frank Carrano
ID: 1071001234
Lecture Section: TC101
Tutorial Section: TT1L
Email: abc123@yourmail.com
Phone: 018-1234567
**********|**********|**********/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
using namespace std;
// Constants for battlefield
const int WIDTH = 10;
const int HEIGHT = 10;
const char EMPTY = '0';
const char ISLAND = '1';
// Battlefield structure
struct Battlefield {
 char grid[HEIGHT][WIDTH];
 // Initialize the battlefield with empty spaces and islands
 void initialize() {
 for (int i = 0; i < HEIGHT; i++) {
 for (int j = 0; j < WIDTH; j++) {
 grid[i][j] = EMPTY;
 }
 }
 // Example: Placing islands (can be replaced with file input)
 grid[1][6] = ISLAND;
 grid[2][6] = ISLAND;
 grid[3][6] = ISLAND;
 }
 // Display the battlefield
 void display() {
 for (int i = 0; i < HEIGHT; i++) {
 for (int j = 0; j < WIDTH; j++) {
 cout << grid[i][j] << " ";
 }
 cout << endl;
 }
 cout << endl;
 }
 // Log the battlefield to a file
 void logToFile(ofstream &logFile) {
 logFile << "Battlefield State:\n";
 for (int i = 0; i < HEIGHT; i++) {
 for (int j = 0; j < WIDTH; j++) {
 logFile << grid[i][j] << " ";
 }
 logFile << endl;
 }
 logFile << endl;
 }
};
// Example usage

class ship
{
protected:
    int lives = 3;
    

public:
    void damage()
    {
        lives--;
    }
    virtual char spawn();//pure virtual so build your own spawn in the ship

bool move(int &a, int &b, char grid[10][10]);
};

class MovingShip : public ship
{
public:
    char spawn() override
    {
        return '<';// example of spawn (can change)
    }
};
class shootingShip : public ship
{
};
class SeeingRobot : public ship
{
};
class RamShip : public ship
{
};

class BattleShip : public MovingShip, public shootingShip, public SeeingRobot
{
};
class Cruiser : public MovingShip, public RamShip, public SeeingRobot
{
};
class Destroyer : public MovingShip, public shootingShip, public RamShip, public SeeingRobot
{
};
class Frigate : public shootingShip
{
};
class Corvette : public shootingShip, public SeeingRobot
{
};
class Amphibious : public MovingShip, public shootingShip, public SeeingRobot
{
};
class Supership : public MovingShip, public shootingShip, public RamShip, public SeeingRobot
{
};

int main() {
 Battlefield battlefield;
 battlefield.initialize();
 // Open log file
 ofstream logFile("battlefield_log.txt");
 if (!logFile) {
 cerr << "Error opening log file!" << endl;
 return 1;
 }
 // Display and log the battlefield
 battlefield.display();
 battlefield.logToFile(logFile);
 logFile.close();
 return 0;
}

bool ship::move(int &a, int &b, char grid[10][10])
    {
        char input;
        cout << endl
             << "input direction(l, r, u, d, e to end): ";
        cin >> input;

        int new_a = a;
        int new_b = b;

        switch (input)
        {
        case 'u':
            new_a = (a > 0) ? a - 1 : a; // Move up
            break;
        case 'd':
            new_a = (a < 9) ? a + 1 : a; // Move down
            break;
        case 'r':
            new_b = (b < 9) ? b + 1 : b; // Move right
            break;
        case 'l':
            new_b = (b > 0) ? b - 1 : b; // Move left
            break;
        case 'e':
            return false; // End the loop
        default:
            cout << "Invalid input!" << endl;
            return true; // Continue the loop
        }

        // Check if the target space is '0'
        if (grid[new_a][new_b] == '0')
        {
            a = new_a;
            b = new_b;
        }
        else
        {
            cout << "Cannot move to the target space, it's not '0'!" << endl;
        }

        return true; // Continue the loop
    }
