#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <list>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

typedef int facing;

struct Ship {
  long nOffset = 0;
  long eOffset = 0;

  int wpNorth = 1;
  int wpEast = 10;

  void North(int offset) {
    wpNorth += offset;
  }
  void East(int offset) {
    wpEast += offset;
  }
  void Turn(int degrees) {
    int rot = (degrees + 360) % 360;
    if (rot == 0) { // NOOP
    } else if (rot == 90) {
      int tmpN = wpNorth;
      wpNorth = -wpEast;
      wpEast = tmpN;
    } else if (rot == 180) {
      wpNorth = - wpNorth;
      wpEast = -wpEast;
    } else if (rot == 270) {
      int tmpN = wpNorth;
      wpNorth = wpEast;
      wpEast = -tmpN;
    } else {
      cout << "Unhandled rotation " << rot << " (turning " << degrees << ")\n";
    }
  }
  void Forward(int spaces) {
    long dE = wpEast * spaces;
    long dN = wpNorth * spaces;
    nOffset += dN;
    eOffset += dE;
  }
  string str() {
    ostringstream out;
    out << "north: " << nOffset << " east: " << eOffset << " wpNorth: " << wpNorth << " wpEast: " << wpEast;
    return out.str();
  }
};

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  Ship s;
  string line;
  while (inFile >> line) {
    char op = line[0];
    int spaces = stoi(line.substr(1));
    if (op == 'N') {
      s.North(spaces);
    } else if (op == 'S') {
      s.North(-spaces);
    } else if (op == 'E') {
      s.East(spaces);
    } else if (op == 'W') {
      s.East(-spaces);
    } else if (op == 'R') {
      s.Turn(spaces);
    } else if (op == 'L') {
      s.Turn(-spaces);
    } else if (op == 'F') {
      s.Forward(spaces);
    } else {
      cout << "Unrecognized op code '" << op << "' in: " << line << "\n";
    }
    // cout << "After " << line << " ship: " << s.str() << "\n";
  }
  inFile.close();

  cout << "Ship ended " << s.nOffset << " north, " << s.eOffset << " east: " << abs(s.nOffset) + abs(s.eOffset) << "\n";

  return 0;
}
