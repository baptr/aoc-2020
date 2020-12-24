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
#include <climits>
#include <bitset>

using namespace std;

struct Tile {
  bool black;
  void Flip() {
    black = !black;
  }
};

ostream& operator<<(ostream& strm, const Tile& t) {
  return strm << (t.black ? "B" : "W");
}

struct Pos {
  int x = 0;
  int y = 0;

  bool operator<(const Pos b) const {
    if (y == b.y) return x < b.x;
    return y < b.y;
  }
};

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  string line;
  map<Pos, Tile> floor;
  while(getline(inFile, line)) {
    Pos p;
    for (size_t i = 0; i < line.length(); ++i) {
      switch (line[i]) {
        case 'e':
          p.x++;
          break;
        case 'w':
          p.x--;
          break;
        case 'n':
          ++i;
          p.y++;
          if (line[i] == 'w') {
              p.x--;
          }
          break;
        case 's':
          ++i;
          p.y--;
          if (line[i] == 'e') {
            p.x++;
          }
          break;
      }
    }
    floor[p].Flip();
    cout << "Flipping (" << p.y << ", " << p.x << ") to " << floor[p] << " via " << line << "\n";
  }

  int numBlack = 0;
  for (auto &[p, t] : floor) {
    if (t.black) numBlack++;
  }

  cout << "Black tiles: " << numBlack << "\n";

  return 0;
}
