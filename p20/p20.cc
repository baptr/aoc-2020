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
#include <queue>

using namespace std;

typedef bitset<10> EdgeHash;

struct Tile {
  uint32_t id;
  EdgeHash n, e, s, w;
  // XXX track mutations internally?
  
  Tile(uint32_t id_, vector<string> rows) {
    id = id_;
    const string& top = rows[0];
    const string& bot = rows[9];
    for (int i = 0; i < 10; ++i) {
      n[9-i] = top[i] == '#';
      s[9-i] = bot[i] == '#';
      e[i] = rows[i][9] == '#';
      w[i] = rows[i][0] == '#';
    }
  }
};

EdgeHash canonicalize(EdgeHash in) {
  EdgeHash flip;
  for (int i = 0; i < 10; ++i) {
    flip[i] = in[9-i];
  }
  if (flip.to_ulong() < in.to_ulong()) {
    return flip;
  }
  return in;
}

enum Dir {NORTH, EAST, SOUTH, WEST};

enum Mut {X_FLIP, Y_FLIP, ROT};
typedef bitset<3> Mutation;

struct EdgeRef {
  Tile* tile;
  // Mutation m; // Flips and rotation required from the tile's given orientation.
  Dir dir; // Canonical edge direction from the original tile definition.
};

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  map<uint32_t,Tile> tiles;
  string line;
  uint32_t id;
  vector<string> rows;
  while(getline(inFile, line)) {
    if (line.empty()) { // assemble
      tiles.emplace(id, Tile{id, rows});
      rows.clear();
    } else if (line.substr(0,4) == "Tile") {
      id = stoi(line.substr(5));
    } else {
      rows.emplace_back(line);
    }
  }
  inFile.close();

  map<uint32_t, vector<EdgeRef>> edgeSets;
  for (auto &[id, t] : tiles) {
    cout << "tiles[" << id << "] = " << t.id << " n=" << t.n.to_ulong() << " e=" << t.e.to_ulong() << " s=" << t.s.to_ulong() << " w=" << t.w.to_ulong() << "\n";
    edgeSets[canonicalize(t.n).to_ulong()].push_back(EdgeRef{&t, NORTH});
    edgeSets[canonicalize(t.s).to_ulong()].push_back(EdgeRef{&t, SOUTH});
    edgeSets[canonicalize(t.w).to_ulong()].push_back(EdgeRef{&t, WEST});
    edgeSets[canonicalize(t.e).to_ulong()].push_back(EdgeRef{&t, EAST});
  }

  for (auto &[id, es] : edgeSets) {
    cout << "edgeSet[" << id << "] (" << es.size() << ") = ";
    for (const EdgeRef& er : es) {
      cout << er.tile->id << " ";
    }
    cout << "\n";
  }

  return 0;
}
