#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <list>
#include <set>
#include <algorithm>

using namespace std;

typedef int space;

const space kFloor = -1;
const space kEmpty = 0;
const space kOccupied = 1;

vector<space> buildRow(string line) {
  vector<space> out;
  for (auto it = line.cbegin(); it != line.cend(); ++it) {
    if (*it == 'L') {
      out.push_back(kEmpty);
    } else {
      out.push_back(kFloor);
    }
  }
  return out;
}

typedef vector<vector<space>> floorMap;

floorMap proc(floorMap prev) {
  floorMap out;
  for (int j = 0; j < prev.size(); ++j) {
    vector<space> row = prev[j];
    vector<space> outRow;
    for (int i = 0; i < row.size(); ++i) {
      int neighbors = 0;
      for (int x = i-1; x <= i+1; ++x) {
        for (int y = j-1; y <= j+1; ++y) {
          if (x < 0 || x >= row.size()) continue;
          if (y < 0 || y >= prev.size()) continue;
          if (x == i && y == j) continue;
          if (prev[y][x] == kOccupied) neighbors++;
        }
      }
      space now = row[i];
      if (now == kEmpty && neighbors == 0) {
        now = kOccupied;
      } else if (now == kOccupied && neighbors >= 4) {
        now = kEmpty;
      }
      outRow.push_back(now);
    }
    out.push_back(outRow);
  }
  return out;
}

// Returns whether the first chair in the viz direction is occupied.
bool occupiedViz(const floorMap& m, int x, int y, int xDir, int yDir) {
  for (;;) {
    x += xDir;
    y += yDir;
    if (y < 0 || y >= m.size()) break;
    vector<space> row = m[y];
    if (x < 0 || x >= row.size()) break;
    space s = row[x];
    if (s == kOccupied) return true;
    if (s == kEmpty) return false;
  }
  return false; // all floor
}

floorMap proc2(const floorMap& prev) {
  floorMap out;
  for (int j = 0; j < prev.size(); ++j) {
    vector<space> row = prev[j];
    vector<space> outRow;
    for (int i = 0; i < row.size(); ++i) {
      space now = row[i];
      if (now == kFloor) {
        outRow.push_back(now);
        continue;
      }
      int neighbors = 0;
      for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
          if (x == 0 && y == 0) continue;
          if (occupiedViz(prev, i, j, x, y)) {
            neighbors++;
            if (neighbors >= 5) break;
          }
        }
      }
      if (now == kEmpty && neighbors == 0) {
        now = kOccupied;
      } else if (now == kOccupied && neighbors >= 5) {
        now = kEmpty;
      }
      outRow.push_back(now);
    }
    out.push_back(outRow);
  }
  return out;
}

void dump(floorMap floor) {
  for (auto row = floor.cbegin(); row != floor.cend(); ++row) {
    for (auto it = row->cbegin(); it != row->cend(); ++it) {
      if (*it == kFloor) {
        cout << ".";
      } else if (*it == kEmpty) {
        cout << "L";
      } else if (*it == kOccupied) {
        cout << "#";
      } else {
        cout << "!";
      }
    }
    cout << "\n";
  }
  cout << "\n";
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  string line;
  floorMap floor;
  while (inFile >> line) {
    floor.push_back(buildRow(line));
  }
  inFile.close();

  for (unsigned int i = 1;; i++) {
    // dump(floor);
    floorMap next = proc2(floor);
    if (next == floor) {
      cout << "Match after " << i << " iterations\n";
      break;
    }
    floor = next;
  }

  int occupied = 0;
  for (auto row = floor.cbegin(); row != floor.cend(); ++row) {
    for (auto it = row->cbegin(); it != row->cend(); ++it) {
      if (*it == kOccupied) occupied++;
    }
  }
  cout << "Seats occupied: " << occupied << "\n";

  return 0;
}
