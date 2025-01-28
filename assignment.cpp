
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

int getRandomNumber(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

int calculateDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Base ship class with kills
class ship {
protected:
    int lives = 3;
    int kills = 0;  // Single kills counter for all derived classes

public:
    void damage() { lives--; }
    bool isAlive() const { return lives > 0; }
    void addKill() { kills++; }
    int getKills() const { return kills; }
    virtual char spawn() = 0;
};

class MovingShip : public virtual ship {
protected:
    virtual bool canMoveToLocation(char targetCell) const { // Mark as virtual
        return targetCell == '0';
    }

public:
    bool move(int &a, int &b, char grid[10][10]); // Existing move implementation
    
    char spawn() override { return '<'; }
    
    pair<int, int> findNeighborPosition(int currentX, int currentY, char grid[10][10]) {
        int newX = currentX + getRandomNumber(-1, 1);
        int newY = currentY + getRandomNumber(-1, 1);
        newX = max(0, min(9, newX));
        newY = max(0, min(9, newY));
        return make_pair(newX, newY);
    }
};

// shootingShip class
class shootingShip : public virtual ship {  // Add virtual inheritance
protected:
    static const int MAX_FIRE_DISTANCE = 5;
    
    bool canShootAt(int fromX, int fromY, int toX, int toY) const {
        return calculateDistance(fromX, fromY, toX, toY) <= MAX_FIRE_DISTANCE;
    }
    
    pair<int, int> getRandomTarget(int currentX, int currentY) {
        int targetX, targetY;
        do {
            targetX = getRandomNumber(0, 9);
            targetY = getRandomNumber(0, 9);
        } while (!canShootAt(currentX, currentY, targetX, targetY));
        
        return make_pair(targetX, targetY);
    }
    
    void fireAt(int targetX, int targetY, char grid[10][10]) {
        if (grid[targetX][targetY] != '0' && grid[targetX][targetY] != '1') {
            addKill();  // Use the base class method
            // Implement actual damage here
        }
    }
};

// SeeingRobot class
class SeeingRobot : public virtual ship {  // Add virtual inheritance
protected:
    bool canSeePosition(int fromX, int fromY, int toX, int toY) const {
        return calculateDistance(fromX, fromY, toX, toY) <= 3;
    }
};

// RamShip class
class RamShip : public virtual ship {  // Add virtual inheritance
protected:
    void ramShip(int targetX, int targetY, char grid[10][10]) {
        if (grid[targetX][targetY] != '0' && grid[targetX][targetY] != '1') {
            addKill();  // Use the base class method
            // Implement ship destruction here
        }
    }
};

// BattleShip class
class BattleShip : public MovingShip, public shootingShip, public SeeingRobot {
private:
    bool isUpgraded = false;

public:
    void performTurn(int currentX, int currentY, char grid[10][10]) {
        // First move
        pair<int, int> moveTarget = findNeighborPosition(currentX, currentY, grid);
        if (canMoveToLocation(grid[moveTarget.first][moveTarget.second])) {
            currentX = moveTarget.first;
            currentY = moveTarget.second;
        }
        
        // Then shoot twice
        for (int i = 0; i < 2; i++) {
            pair<int, int> target = getRandomTarget(currentX, currentY);
            fireAt(target.first, target.second, grid);
        }
        
        // Check for upgrade to Destroyer
        if (getKills() >= 4) isUpgraded = true;
    }
    
    char spawn() override { return 'B'; }
};

// Cruiser class
class Cruiser : public MovingShip, public RamShip, public SeeingRobot {
private:
    bool isUpgraded = false;

public:
    void performTurn(int currentX, int currentY, char grid[10][10]) {
        // Look for ships in 3x3 neighborhood
        for (int i = max(0, currentX-1); i <= min(9, currentX+1); i++) {
            for (int j = max(0, currentY-1); j <= min(9, currentY+1); j++) {
                if (grid[i][j] != '0' && grid[i][j] != '1') {
                    ramShip(i, j, grid);
                    return;
                }
            }
        }
        
        // If no ships found, move randomly in neighborhood
        pair<int, int> moveTarget = findNeighborPosition(currentX, currentY, grid);
        if (canMoveToLocation(grid[moveTarget.first][moveTarget.second])) {
            currentX = moveTarget.first;
            currentY = moveTarget.second;
        }
        
        // Check for upgrade to Destroyer
        if (getKills() >= 3) isUpgraded = true;
    }
    
    char spawn() override { return 'C'; }
};

// Destroyer class
class Destroyer : public MovingShip, public shootingShip, public RamShip, public SeeingRobot {
private:
    bool isUpgraded = false;

public:
    void performTurn(int currentX, int currentY, char grid[10][10]) {
        // First try to ram nearby ships
        for (int i = max(0, currentX-1); i <= min(9, currentX+1); i++) {
            for (int j = max(0, currentY-1); j <= min(9, currentY+1); j++) {
                if (grid[i][j] != '0' && grid[i][j] != '1') {
                    ramShip(i, j, grid);
                    break;
                }
            }
        }
        
        // Then shoot twice like Battleship
        for (int i = 0; i < 2; i++) {
            pair<int, int> target = getRandomTarget(currentX, currentY);
            fireAt(target.first, target.second, grid);
        }
        
        // Check for upgrade to SuperShip
        if (getKills() >= 3) isUpgraded = true;
    }
    
    char spawn() override { return 'D'; }
};

// Frigate class
class Frigate : public shootingShip {
private:
    int currentDirection = 0;  // 0=up, 1=right, 2=down, 3=left

public:
    void performTurn(int currentX, int currentY, char grid[10][10]) {
        const int dx[] = {-1, 0, 1, 0};
        const int dy[] = {0, 1, 0, -1};
        
        int targetX = currentX + dx[currentDirection];
        int targetY = currentY + dy[currentDirection];
        
        if (targetX >= 0 && targetX < 10 && targetY >= 0 && targetY < 10) {
            fireAt(targetX, targetY, grid);
        }
        
        currentDirection = (currentDirection + 1) % 4;
    }
    
    char spawn() override { return 'F'; }
};

// Corvette class
class Corvette : public shootingShip, public SeeingRobot {
public:
    void performTurn(int currentX, int currentY, char grid[10][10]) {
        int dx = getRandomNumber(-1, 1);
        int dy = getRandomNumber(-1, 1);
        
        int targetX = max(0, min(9, currentX + dx));
        int targetY = max(0, min(9, currentY + dy));
        
        if (targetX != currentX || targetY != currentY) {
            fireAt(targetX, targetY, grid);
        }
    }
    
    char spawn() override { return 'V'; }
};

// Amphibious class
class Amphibious : public MovingShip, public shootingShip, public SeeingRobot {
private:
    bool isUpgraded = false;

protected:
    bool canMoveToLocation(char targetCell) const override { // Properly overrides the base class function
        return targetCell == '0' || targetCell == '1';
    }

public:
    void performTurn(int currentX, int currentY, char grid[10][10]) {
        pair<int, int> moveTarget = findNeighborPosition(currentX, currentY, grid);
        if (canMoveToLocation(grid[moveTarget.first][moveTarget.second])) {
            currentX = moveTarget.first;
            currentY = moveTarget.second;
        }
        
        for (int i = 0; i < 2; i++) {
            pair<int, int> target = getRandomTarget(currentX, currentY);
            fireAt(target.first, target.second, grid);
        }
        
        if (getKills() >= 4) isUpgraded = true;
    }
    
    char spawn() override { return 'A'; }
};

// Supership class
class Supership : public MovingShip, public shootingShip, public RamShip, public SeeingRobot {
public:
    void performTurn(int currentX, int currentY, char grid[10][10]) {
        // Move like Cruiser and destroy ships in path
        for (int i = max(0, currentX-1); i <= min(9, currentX+1); i++) {
            for (int j = max(0, currentY-1); j <= min(9, currentY+1); j++) {
                if (grid[i][j] != '0' && grid[i][j] != '1') {
                    ramShip(i, j, grid);
                    currentX = i;
                    currentY = j;
                    break;
                }
            }
        }
        
        // Shoot at 3 random locations
        for (int i = 0; i < 3; i++) {
            int targetX = getRandomNumber(0, 9);
            int targetY = getRandomNumber(0, 9);
            fireAt(targetX, targetY, grid);
        }
    }
    
    char spawn() override { return 'S'; }
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

bool MovingShip::move(int &a, int &b, char grid[10][10])
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
