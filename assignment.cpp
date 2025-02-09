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
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()
#include <random>    // For random_device, mt19937, uniform_int_distribution
#include <algorithm> // For min and max
#include <cctype>
using namespace std;

// Constants for battlefield
const int WIDTH = 10;
const int HEIGHT = 10;
const char EMPTY = '0';
const char ISLAND = '1';

struct Battlefield
{
    char grid[HEIGHT][WIDTH];

    // Initialize the battlefield by reading from a file
    void initialize(const string &filename)
    {
        ifstream inFile(filename);
        if (!inFile)
        {
            cerr << "Error opening file: " << filename << endl;
            exit(1);
        }

        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                inFile >> grid[i][j]; // Read each character into the grid
            }
        }

        inFile.close();
    }

    // Display the battlefield
    void display()
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void logToFile(ofstream &logFile)
    {
        logFile << "Battlefield State:\n";
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                logFile << grid[i][j] << " ";
            }
            logFile << endl;
        }
        logFile << endl;
    }
};
struct Team
{
    int shipCount;
    vector<string> ships;
};

void parseShipData(const string &filename, int &iterations, Team &teamA, Team &teamB)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    Team *currentTeam = nullptr;
    bool isReadingShips = true; // Flag to stop reading ships when the grid starts

    while (getline(file, line))
    {
        istringstream iss(line);
        string firstWord;
        iss >> firstWord; // Read the first word

        // Stop processing ship data if the first word is a digit (grid starts)
        if (isdigit(firstWord[0]))
        {
            break;
        }

        if (firstWord == "iterations")
        {
            iss >> iterations;
        }
        else if (firstWord == "Team")
        {
            string teamName;
            int shipCount;
            iss >> teamName >> shipCount;

            if (teamName == "A")
            {
                teamA.shipCount = shipCount;
                currentTeam = &teamA;
            }
            else if (teamName == "B")
            {
                teamB.shipCount = shipCount;
                currentTeam = &teamB;
            }
        }
        else if (currentTeam && isReadingShips)
        {
            string shipName = firstWord;
            string extra;

            // Ignore any additional symbols or numbers after the ship name
            while (iss >> extra)
            {
                if (isdigit(extra[0]))
                    break; // Stop if a number is encountered
            }

            currentTeam->ships.push_back(shipName);
        }
    }

    file.close();
}
int getRandomNumber(int min, int max)
{
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

int calculateDistance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

// Base ship class with kills
class ship
{
protected:
    int lives = 3;
    int kills = 0; // Single kills counter for all derived classes

public:
    void damage() { lives--; }
    bool isAlive() const { return lives > 0; }
    void addKill() { kills++; }
    int getKills() const { return kills; }
    virtual char spawn() = 0;
};

// MovingShip class (provides movement-related functionality)
class MovingShip : public virtual ship
{
protected:
    virtual bool canMoveToLocation(char targetCell) const
    {
        // By default, a ship can move only into an empty cell.
        return targetCell == EMPTY;
    }

public:
    // This function is provided to allow manual movement
    bool move(int &a, int &b, char grid[HEIGHT][WIDTH])
    {
        char input;
        cout << "\nInput direction (l, r, u, d, e to end): ";
        cin >> input;

        int new_a = a;
        int new_b = b;

        switch (input)
        {
        case 'u':
            new_a = (a > 0) ? a - 1 : a;
            break;
        case 'd':
            new_a = (a < HEIGHT - 1) ? a + 1 : a;
            break;
        case 'r':
            new_b = (b < WIDTH - 1) ? b + 1 : b;
            break;
        case 'l':
            new_b = (b > 0) ? b - 1 : b;
            break;
        case 'e':
            return false; // End movement
        default:
            cout << "Invalid input!" << endl;
            return true;
        }

        // Check if the target space is empty
        if (grid[new_a][new_b] == EMPTY)
        {
            a = new_a;
            b = new_b;
        }
        else
        {
            cout << "Cannot move to the target space; it is not empty!" << endl;
        }
        return true;
    }

    char spawn() override { return '<'; }

    // Returns a neighboring position (could be used for random movement)
    pair<int, int> findNeighborPosition(int currentX, int currentY, char grid[HEIGHT][WIDTH])
    {
        int newX = currentX + getRandomNumber(-1, 1);
        int newY = currentY + getRandomNumber(-1, 1);
        newX = max(0, min(HEIGHT - 1, newX));
        newY = max(0, min(WIDTH - 1, newY));
        return make_pair(newX, newY);
    }
};

// shootingShip class (provides shooting functionality)
class shootingShip : public virtual ship
{
protected:
    static const int MAX_FIRE_DISTANCE = 5;

    bool canShootAt(int fromX, int fromY, int toX, int toY) const
    {
        return calculateDistance(fromX, fromY, toX, toY) <= MAX_FIRE_DISTANCE;
    }

    pair<int, int> getRandomTarget(int currentX, int currentY)
    {
        int targetX, targetY;
        do
        {
            targetX = getRandomNumber(0, HEIGHT - 1);
            targetY = getRandomNumber(0, WIDTH - 1);
        } while (!canShootAt(currentX, currentY, targetX, targetY));
        return make_pair(targetX, targetY);
    }

    void fireAt(int targetX, int targetY, char grid[HEIGHT][WIDTH])
    {
        // For simplicity, if the target cell does not hold an empty cell or island,
        // we count it as a kill.
        if (grid[targetX][targetY] != EMPTY && grid[targetX][targetY] != ISLAND)
        {
            addKill();
            // Optionally, mark the target cell as hit.
            grid[targetX][targetY] = '*';
        }
    }
};

// SeeingRobot class (provides “sight” capability)
class SeeingRobot : public virtual ship
{
protected:
    bool canSeePosition(int fromX, int fromY, int toX, int toY) const
    {
        return calculateDistance(fromX, fromY, toX, toY) <= 3;
    }
};

// RamShip class (provides ramming functionality)
class RamShip : public virtual ship
{
protected:
    void ramShip(int targetX, int targetY, char grid[HEIGHT][WIDTH])
    {
        if (grid[targetX][targetY] != EMPTY && grid[targetX][targetY] != ISLAND)
        {
            addKill();
            // Optionally mark the cell to show damage.
            grid[targetX][targetY] = '*';
        }
    }
};

// BattleShip class: combines moving, shooting, and seeing.
class BattleShip : public MovingShip, public shootingShip, public SeeingRobot
{
private:
    bool isUpgraded = false;

public:
    void performTurn(int &currentX, int &currentY, char grid[HEIGHT][WIDTH])
    {
        // Try to move first.
        pair<int, int> moveTarget = findNeighborPosition(currentX, currentY, grid);
        if (canMoveToLocation(grid[moveTarget.first][moveTarget.second]))
        {
            currentX = moveTarget.first;
            currentY = moveTarget.second;
        }
        // Shoot twice.
        for (int i = 0; i < 2; i++)
        {
            pair<int, int> target = getRandomTarget(currentX, currentY);
            fireAt(target.first, target.second, grid);
        }
        // Upgrade condition example.
        if (getKills() >= 4)
            isUpgraded = true;
    }

    char spawn() override { return 'B'; }
};

// Cruiser class: moves and rams.
class Cruiser : public MovingShip, public RamShip, public SeeingRobot
{
private:
    bool isUpgraded = false;

public:
    void performTurn(int &currentX, int &currentY, char grid[HEIGHT][WIDTH])
    {
        // Look for ships in the 3x3 neighborhood to ram.
        for (int i = max(0, currentX - 1); i <= min(HEIGHT - 1, currentX + 1); i++)
        {
            for (int j = max(0, currentY - 1); j <= min(WIDTH - 1, currentY + 1); j++)
            {
                if (grid[i][j] != EMPTY && grid[i][j] != ISLAND)
                {
                    ramShip(i, j, grid);
                    return; // Only ram one target per turn.
                }
            }
        }
        // If no target, move randomly.
        pair<int, int> moveTarget = findNeighborPosition(currentX, currentY, grid);
        if (canMoveToLocation(grid[moveTarget.first][moveTarget.second]))
        {
            currentX = moveTarget.first;
            currentY = moveTarget.second;
        }
        if (getKills() >= 3)
            isUpgraded = true;
    }

    char spawn() override { return 'C'; }
};

// Destroyer class: combines moving, shooting, and ramming.
class Destroyer : public MovingShip, public shootingShip, public RamShip, public SeeingRobot
{
private:
    bool isUpgraded = false;

public:
    void performTurn(int &currentX, int &currentY, char grid[HEIGHT][WIDTH])
    {
        // Try to ram a nearby ship first.
        for (int i = max(0, currentX - 1); i <= min(HEIGHT - 1, currentX + 1); i++)
        {
            for (int j = max(0, currentY - 1); j <= min(WIDTH - 1, currentY + 1); j++)
            {
                if (grid[i][j] != EMPTY && grid[i][j] != ISLAND)
                {
                    ramShip(i, j, grid);
                    break;
                }
            }
        }
        // Then shoot twice.
        for (int i = 0; i < 2; i++)
        {
            pair<int, int> target = getRandomTarget(currentX, currentY);
            fireAt(target.first, target.second, grid);
        }
        if (getKills() >= 3)
            isUpgraded = true;
    }

    char spawn() override { return 'D'; }
};

// Frigate class: shoots in a fixed direction that rotates.
class Frigate : public shootingShip
{
private:
    int currentDirection = 0; // 0=up, 1=right, 2=down, 3=left

public:
    void performTurn(int currentX, int currentY, char grid[HEIGHT][WIDTH])
    {
        const int dx[] = {-1, 0, 1, 0};
        const int dy[] = {0, 1, 0, -1};

        int targetX = currentX + dx[currentDirection];
        int targetY = currentY + dy[currentDirection];

        if (targetX >= 0 && targetX < HEIGHT && targetY >= 0 && targetY < WIDTH)
        {
            fireAt(targetX, targetY, grid);
        }
        currentDirection = (currentDirection + 1) % 4;
    }

    char spawn() override { return 'F'; }
};

// Corvette class: shoots at a randomly chosen neighboring cell.
class Corvette : public shootingShip, public SeeingRobot
{
public:
    void performTurn(int currentX, int currentY, char grid[HEIGHT][WIDTH])
    {
        int dx = getRandomNumber(-1, 1);
        int dy = getRandomNumber(-1, 1);
        int targetX = max(0, min(HEIGHT - 1, currentX + dx));
        int targetY = max(0, min(WIDTH - 1, currentY + dy));

        if (targetX != currentX || targetY != currentY)
        {
            fireAt(targetX, targetY, grid);
        }
    }

    char spawn() override { return 'V'; }
};

// Amphibious class: can move onto islands as well as empty water.
class Amphibious : public MovingShip, public shootingShip, public SeeingRobot
{
private:
    bool isUpgraded = false;

protected:
    bool canMoveToLocation(char targetCell) const override
    {
        return targetCell == EMPTY || targetCell == ISLAND;
    }

public:
    void performTurn(int &currentX, int &currentY, char grid[HEIGHT][WIDTH])
    {
        pair<int, int> moveTarget = findNeighborPosition(currentX, currentY, grid);
        if (canMoveToLocation(grid[moveTarget.first][moveTarget.second]))
        {
            currentX = moveTarget.first;
            currentY = moveTarget.second;
        }
        for (int i = 0; i < 2; i++)
        {
            pair<int, int> target = getRandomTarget(currentX, currentY);
            fireAt(target.first, target.second, grid);
        }
        if (getKills() >= 4)
            isUpgraded = true;
    }

    char spawn() override { return 'A'; }
};

// Supership class: advanced ship that uses all capabilities.
class Supership : public MovingShip, public shootingShip, public RamShip, public SeeingRobot
{
public:
    void performTurn(int &currentX, int &currentY, char grid[HEIGHT][WIDTH])
    {
        // Try to ram a nearby ship and then move to its location.
        for (int i = max(0, currentX - 1); i <= min(HEIGHT - 1, currentX + 1); i++)
        {
            for (int j = max(0, currentY - 1); j <= min(WIDTH - 1, currentY + 1); j++)
            {
                if (grid[i][j] != EMPTY && grid[i][j] != ISLAND)
                {
                    ramShip(i, j, grid);
                    currentX = i;
                    currentY = j;
                    break;
                }
            }
        }
        // Then shoot at 3 random locations.
        for (int i = 0; i < 3; i++)
        {
            int targetX = getRandomNumber(0, HEIGHT - 1);
            int targetY = getRandomNumber(0, WIDTH - 1);
            fireAt(targetX, targetY, grid);
        }
    }

    char spawn() override { return 'S'; }
};

// -------------------------------------------------
// Suitable main function demonstrating the simulation
// -------------------------------------------------
int main()
{
    srand(time(0)); // Seed the C random number generator (if needed)

    Battlefield battlefield;
    string filename = "battlefield.txt";
    battlefield.initialize(filename);
    int iterations;
    Team teamA, teamB;
    parseShipData("game.txt", iterations, teamA, teamB);

    // Create a fleet of ships (one of each type)
    vector<ship *> fleet;
    // Note: When using multiple inheritance, the most-derived type is created.
    BattleShip *battleship = new BattleShip();
    Cruiser *cruiser = new Cruiser();
    Destroyer *destroyer = new Destroyer();
    Frigate *frigate = new Frigate();
    Corvette *corvette = new Corvette();
    Amphibious *amphibious = new Amphibious();
    Supership *supership = new Supership();

    fleet.push_back(battleship);
    fleet.push_back(cruiser);
    fleet.push_back(destroyer);
    fleet.push_back(frigate);
    fleet.push_back(corvette);
    fleet.push_back(amphibious);
    fleet.push_back(supership);

    // For each ship, choose a random empty location on the battlefield and place it.
    // We also keep track of each ship's current position.
    vector<pair<int, int>> positions;
    for (size_t i = 0; i < teamA.ships.size(); i++)
    {
        int x, y;
        do
        {
            x = getRandomNumber(0, HEIGHT - 1);
            y = getRandomNumber(0, WIDTH - 1);
        } while (battlefield.grid[x][y] != EMPTY);
        positions.push_back(make_pair(x, y));
        if (teamA.ships[i] == "Battleship")
            battlefield.grid[x][y] = battleship->spawn();
        else if (teamA.ships[i] == "Cruiser")
            battlefield.grid[x][y] = cruiser->spawn();
        else if (teamA.ships[i] == "Destroyer")
            battlefield.grid[x][y] = destroyer->spawn();
        else if (teamA.ships[i] == "Frigate")
            battlefield.grid[x][y] = frigate->spawn();
        else if (teamA.ships[i] == "Corvette")
            battlefield.grid[x][y] = corvette->spawn();
        else if (teamA.ships[i] == "Amphibious")
            battlefield.grid[x][y] = amphibious->spawn();
        else if (teamA.ships[i] == "Supership")
            battlefield.grid[x][y] = supership->spawn();
    }

    for (size_t i = 0; i < teamB.ships.size(); i++)
    {
        int x, y;
        do
        {
            x = getRandomNumber(0, HEIGHT - 1);
            y = getRandomNumber(0, WIDTH - 1);
        } while (battlefield.grid[x][y] != EMPTY);
        positions.push_back(make_pair(x, y));
        if (teamB.ships[i] == "Battleship")
            battlefield.grid[x][y] = battleship->spawn();
        else if (teamB.ships[i] == "Cruiser")
            battlefield.grid[x][y] = cruiser->spawn();
        else if (teamB.ships[i] == "Destroyer")
            battlefield.grid[x][y] = destroyer->spawn();
        else if (teamB.ships[i] == "Frigate")
            battlefield.grid[x][y] = frigate->spawn();
        else if (teamB.ships[i] == "Corvette")
            battlefield.grid[x][y] = corvette->spawn();
        else if (teamB.ships[i] == "Amphibious")
            battlefield.grid[x][y] = amphibious->spawn();
        else if (teamB.ships[i] == "Supership")
            battlefield.grid[x][y] = supership->spawn();
    }

    cout << "Initial Battlefield:" << endl;
    battlefield.display();

    // Open a log file to log the battlefield state
    ofstream logFile("battlefield_log.txt");

    // Simulation: run several turns

    for (int turn = 0; turn < iterations; turn++)
    {
        // Log the current state to the file after each turn
        battlefield.logToFile(logFile);

        // Clear the battlefield cells that are not islands.
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (battlefield.grid[i][j] != ISLAND)
                    battlefield.grid[i][j] = EMPTY;
            }
        }
        // For each ship, perform its turn.
        // Note: Only ships that use movement will update their stored positions.
        for (int i = 0; i < fleet.size(); i++)
        {
            for (int j = 0; j < teamA.ships.size(); j++)
            {
                // Use dynamic_cast to check if the ship has a performTurn that accepts position by reference.
                // For those that do, we pass the stored position so that movement is recorded.
                // (For example, Frigate and Corvette do not update their positions.)
                if (BattleShip *bs = dynamic_cast<BattleShip *>(fleet[i]))
                {
                    if ("Battleship" == teamA.ships[j]) //&& ship exist on list
                    {
                        if (bs->isAlive())
                            bs->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                        else // Ship is destroyed
                            battlefield.grid[positions[i].first][positions[i].second] = EMPTY;
                    }
                }
                else if (Cruiser *cr = dynamic_cast<Cruiser *>(fleet[i]))
                {
                    if ("Cruiser" == teamA.ships[j]) //&& ship exist on list
                    {
                        if (cr->isAlive())
                            cr->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                        else // Ship is destroyed
                            battlefield.grid[positions[i].first][positions[i].second] = EMPTY;
                    }
                }
                else if (Destroyer *ds = dynamic_cast<Destroyer *>(fleet[i]))
                {
                    if ("Destroyer" == teamA.ships[j]) //&& ship exist on list
                    {
                        if (ds->isAlive())
                            ds->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                        else // Ship is destroyed
                            battlefield.grid[positions[i].first][positions[i].second] = EMPTY;
                    }
                }
                else if (Amphibious *am = dynamic_cast<Amphibious *>(fleet[i]))
                {
                    if ("Amphibious" == teamA.ships[j]) //&& ship exist on list
                    {
                        if (am->isAlive())
                            am->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                        else // Ship is destroyed
                            battlefield.grid[positions[i].first][positions[i].second] = EMPTY;
                    }
                }
                else if (Supership *ss = dynamic_cast<Supership *>(fleet[i]))
                {
                    if ("Supership" == teamA.ships[j]) //&& ship exist on list
                    {
                            if (ss->isAlive())
                                ss->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                            else // Ship is destroyed
                                battlefield.grid[positions[i].first][positions[i].second] = EMPTY;
                    }
                }
                else if (Frigate *fg = dynamic_cast<Frigate *>(fleet[i]))
                {
                    if ("Frigate" == teamA.ships[j]) //&& ship exist on list
                    {
                        if (fg->isAlive())
                            fg->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                        else // Ship is destroyed
                            battlefield.grid[positions[i].first][positions[i].second] = EMPTY;
                    }
                }
                else if (Corvette *cv = dynamic_cast<Corvette *>(fleet[i]))
                {
                    if ("Corvette" == teamA.ships[j]) //&& ship exist on list
                    {
                        if (cv->isAlive())
                            cv->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                        else // Ship is destroyed
                            battlefield.grid[positions[i].first][positions[i].second] = EMPTY;
                    }
                }

                // Place the ship on the battlefield using its spawn symbol.
                if (fleet[i]->isAlive())
                    battlefield.grid[positions[i].first][positions[i].second] = fleet[i]->spawn();
            }
            for (int j = 0; j < teamB.ships.size(); j++)
            {
                // Use dynamic_cast to check if the ship has a performTurn that accepts position by reference.
                // For those that do, we pass the stored position so that movement is recorded.
                // (For example, Frigate and Corvette do not update their positions.)
                if (BattleShip *bs = dynamic_cast<BattleShip *>(fleet[i]))
                {
                    if ("Battleship" == teamB.ships[j]) //&& ship exist on list
                    {
                        bs->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                    }
                }
                else if (Cruiser *cr = dynamic_cast<Cruiser *>(fleet[i]))
                {
                    if ("Cruiser" == teamB.ships[j]) //&& ship exist on list
                    {
                        cr->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                    }
                }
                else if (Destroyer *ds = dynamic_cast<Destroyer *>(fleet[i]))
                {
                    if ("Destroyer" == teamB.ships[j]) //&& ship exist on list
                    {
                        ds->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                    }
                }
                else if (Amphibious *am = dynamic_cast<Amphibious *>(fleet[i]))
                {
                    if ("Amphibious" == teamB.ships[j]) //&& ship exist on list
                    {
                        am->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                    }
                }
                else if (Supership *ss = dynamic_cast<Supership *>(fleet[i]))
                {
                    if ("Supership" == teamB.ships[j]) //&& ship exist on list
                    {
                        ss->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                    }
                }
                else if (Frigate *fg = dynamic_cast<Frigate *>(fleet[i]))
                {
                    if ("Frigate" == teamB.ships[j]) //&& ship exist on list
                    {
                        fg->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                    }
                }
                else if (Corvette *cv = dynamic_cast<Corvette *>(fleet[i]))
                {
                    if ("Corvette" == teamB.ships[j]) //&& ship exist on list
                    {
                        cv->performTurn(positions[i].first, positions[i].second, battlefield.grid);
                    }
                }
                battlefield.grid[positions[i].first][positions[i].second] = fleet[i]->spawn();
            }
        }
        cout << "After Turn " << turn + 1 << ":" << endl;
        battlefield.display();
    }

    // Close the log file
    logFile.close();

    // Clean up dynamically allocated ships
    for (auto s : fleet)
    {
        delete s;
    }

    return 0;
}
