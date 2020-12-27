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

EdgeHash reverse(const EdgeHash& in) {
  EdgeHash out;
  for (int i = 0; i < 10; ++i) {
    out[i] = in[9-i];
  }
  return out;
}

enum Dir {NORTH, EAST, SOUTH, WEST};

ostream& operator<<(ostream& strm, Dir d) {
  if (d == NORTH) {
    return strm << "N";
  } else if (d == EAST) {
    return strm << "E";
  } else if (d == SOUTH) {
    return strm << "S";
  } else if (d == WEST) {
    return strm << "W";
  }
  return strm << "<invalid DIR>";
}

struct Tile {
  uint32_t id;
  EdgeHash n, e, s, w;
  bitset<8> image[8]; // image[y][x]
  
  Tile(uint32_t id_, vector<string> rows) {
    id = id_;
    const string& top = rows[0];
    const string& bot = rows[9];
    for (int i = 0; i < 10; ++i) {
      n[9-i] = top[i] == '#';
      s[9-i] = bot[i] == '#';
      e[9-i] = rows[i][9] == '#';
      w[9-i] = rows[i][0] == '#';
    }
  }

  void flipX() {
    EdgeHash w_ = w;
    w = e;
    e = w_;
    n = reverse(n);
    s = reverse(s);
  }

  void flipY() {
    EdgeHash n_ = n;
    n = s;
    s = n_;
    w = reverse(w);
    e = reverse(e);
  }

  void orient(Dir src, Dir dst) {
    if (src == dst) {
      return;
    } else if ((src+1)%4 == dst) { // rotate CW
      cout << "Rotating " << id << " CW from " << src << " to " << dst << "\n";
      EdgeHash w_ = w;
      w = s;
      s = reverse(e);
      e = n;
      n = reverse(w_);
    } else if ((src+3)%4 == dst) { // rotate CCW
      cout << "Rotating " << id << " CCW from " << src << " to " << dst << "\n";
      EdgeHash w_ = w;
      w = reverse(n);
      n = e;
      e = reverse(s);
      s = w_;
    } else if ((src == EAST && dst == WEST) || (src == WEST && dst == EAST)) {
      cout << "Flipping " << id << " over X from " << src << " to " << dst << "\n";
      flipX();
    } else if ((src == NORTH && dst == SOUTH) || (src == SOUTH && dst == NORTH)) {
      cout << "Flipping " << id << " over Y from " << src << " to " << dst << "\n";
      flipY();
    } else {
      cout << "Unhandled rotation of " << id << " from " << src << " to " << dst << "\n";
    }
  }

  // Orient this tile to have the provided (non-canonical) EdgeHash on the
  // specified face.
  void match(Dir src, Dir face, const EdgeHash want) {
    orient(src, face);
    if (face == NORTH) {
      if (n != want) flipX();
      if (n != want) cout << "Failed to match Tile[" << id << "].n = " << n << " to " << want << "\n";
    } else if (face == SOUTH) {
      if (s != want) flipX();
      if (s != want) cout << "Failed to match Tile[" << id << "].s = " << s << " to " << want << "\n";
    } else if (face == EAST) {
      if (e != want) flipY();
      if (e != want) cout << "Failed to match Tile[" << id << "].e = " << e << " to " << want << "\n";
    } else if (face == WEST) {
      if (w != want) flipY();
      if (w != want) cout << "Failed to match Tile[" << id << "].w = " << w << " to " << want << "\n";
    }
  }
};

EdgeHash canonical(const EdgeHash& in) {
  EdgeHash flip = reverse(in);
  if (flip.to_ulong() < in.to_ulong()) {
    return flip;
  }
  return in;
}

uint32_t canonID(EdgeHash in) {
  return canonical(in).to_ulong();
}

struct EdgeRef {
  Tile* tile;
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
    cout << "tiles[" << id << "] = " << t.id << " n=" << t.n << " e=" << t.e << " s=" << t.s << " w=" << t.w << "\n";
    edgeSets[canonID(t.n)].push_back(EdgeRef{&t, NORTH});
    edgeSets[canonID(t.s)].push_back(EdgeRef{&t, SOUTH});
    edgeSets[canonID(t.w)].push_back(EdgeRef{&t, WEST});
    edgeSets[canonID(t.e)].push_back(EdgeRef{&t, EAST});
  }

  for (auto &[id, es] : edgeSets) {
    cout << "edgeSet[" << id << "] (" << es.size() << ") = ";
    for (const EdgeRef& er : es) {
      cout << er.tile->id << " [" << er.dir << "] ";
    }
    cout << "\n";
  }
  
  // Since (grep) analysis of the canonicalized edge sets shows that all edge
  // pairs unambiguously match 1:1, we can just pick a corner (one of the four
  // tiles with two unmatched edges), and build out from there. Then we'll have
  // to try all 2^3 orientations of the full picture while looking for snakes.
  const uint32_t cornerID = 1093; // externally obtained for full_input.txt
  
  // Though we need to make sure the useful corners are "in"...
  Tile& corner = tiles.at(cornerID);
  if (edgeSets.at(canonical(corner.s).to_ulong()).size() == 1) {
    corner.flipY();
  }
  if (edgeSets.at(canonical(corner.e).to_ulong()).size() == 1) {
    corner.flipX();
  }

  cout << "Corner[" << cornerID << "].east = " << corner.e << " (" << canonID(corner.e) << ")\n";
  cout << "Corner[" << cornerID << "].south = " << corner.s << " (" << canonID(corner.s) << ")\n";

  uint32_t grid[12][12];
  grid[0][0] = cornerID;
  for (int j = 0; j < 12; ++j) {
    if (j > 0) {
      // Match the first in the row to the tile above.
      const Tile& up = tiles.at(grid[j-1][0]);
      cout << "Matching grid[" << j-1 << "][0](" << grid[j-1][0] << ").s = " << up.s << " (" << canonID(up.s) << ")...\n";
      const vector<EdgeRef>& refSet = edgeSets.at(canonID(up.s));
      uint32_t next_id = -1;
      for (const auto& r : refSet) {
        if (r.tile->id == up.id) continue;
        if (next_id != -1) {
          cout << "Duplicate matches for grid[" << j << "][0]! refSet(" << canonID(up.s) << ") = " << refSet.size() << "\n";
          exit(1);
        }
        next_id = r.tile->id;
        r.tile->match(r.dir, NORTH, up.s);
        break;
      }
      if (next_id == -1) {
        cout << "Unable to match grid[" << j << "][0]! refSet(" << canonID(up.s) << ") = " << refSet.size() << "\n";
        exit(1);
      }
      grid[j][0] = next_id;
    }
    for (int i = 1; i < 12; ++i) {
      // Match the rest of the row to the tile before.
      const Tile& left = tiles.at(grid[j][i-1]);
      cout << "Matching grid[" << j << "][" << i-1 << "](" << grid[j][i-1] << ").e = " << left.e << " (" << canonID(left.e) << ")...\n";
      const vector<EdgeRef>& refSet = edgeSets.at(canonID(left.e));
      uint32_t next_id = -1;
      for (const auto& r : refSet) {
        if (r.tile->id == left.id) continue;
        next_id = r.tile->id;
        r.tile->match(r.dir, WEST, left.e);
        break;
      }
      if (next_id == -1) {
        cout << "Unable to match grid[" << j << "][" << i << "]! refSet(" << canonID(left.e) << ") = " << refSet.size() << "\n";
        exit(1);
      }
      grid[j][i] = next_id;
      if (j > 0) {
        cout << "Check grid[" << j-1 << "][" << i << "](" << grid[j-1][i] << ").s = " << tiles.at(grid[j-1][i]).s << " == down.n = " << tiles.at(grid[j][i]).n << "\n";
      }
    }
  }

  cout << "\nLayout:\n";
  for (int j = 0; j < 12; ++j) {
    for (int i = 0; i < 12; ++i) {
      cout << grid[j][i] << " ";
    }
    cout << "\n";
  }

  return 0;
}
