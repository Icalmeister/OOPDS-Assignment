
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
