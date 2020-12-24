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

  vector<Pos> neighbors() const {
    return {
      {x-1, y},
        {x+1, y},
        {x, y+1},
        {x-1, y+1},
        {x, y-1},
        {x+1, y-1},
    };
  }
};

const set<Pos> checkList(const map<Pos, Tile>& floor) {
  set<Pos> out;
  for (auto &[p, t] : floor) {
    if (t.black) {
      out.emplace(p);
      const auto n = p.neighbors();
      out.insert(n.begin(), n.end());
    }
  }
  return out;
}

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

  for (int day = 1; day <= 100; ++day) {
    // since the floor structure is sparse and something like 40x40, it feels
    // wrong to iterate over the whole thing.  ~ugly option: make a todo list
    // of positions of every black tile and everything adjacent to them.
    // then step through that list, checking rules.
    const set<Pos>& check = checkList(floor);
    map<Pos,Tile> next;
    int blackTomorrow = 0;
    for (const Pos& p : check) {
      int count = 0;
      if (floor[p].black) { // 0 or more than 2 -> flip to white
        for (const Pos& n : p.neighbors()) {
          if (floor[n].black) {
            count++;
            if (count > 2) break;
          }
        }
        if (count == 0 || count > 2) {
          // flip to white.. no need to mention it
        } else { // stays black tomorrow
          next[p] = Tile{true};
          blackTomorrow++;
        }
      } else { // exactly 2 -> flip to black
        int count = 0;
        for (const Pos& n : p.neighbors()) {
          if (floor[n].black) count++;
        }
        if (count == 2) {
          next[p] = Tile{true};
          blackTomorrow++;
        }
      }
    }
    cout << "Day[" << day << "] : checking " << check.size() << " tiles, " << blackTomorrow << " will be black\n";
    floor = next;
  }

  return 0;
}
