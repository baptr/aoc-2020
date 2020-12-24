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

// full input is 8x8x1.
// 6 steps can only expand in each direction by 6, so max of 20x20x13
// XXX hmm.. and the space is always mirrored across the starer plane.. though
// it seems complicated to utilize that fact
#define maxDim 32

struct Grid {
  bitset<maxDim> space[maxDim][maxDim]; // space[z][x][y]

  int minX, maxX;
  int minY, maxY;
  int minZ, maxZ;
};

ostream& operator<<(ostream& strm, const Grid& g) {
  strm << "Bounds: z: [" << g.minZ << ", " << g.maxZ << "] y: [" << g.minY << ", " << g.maxY << "] x: [" << g.minX << ", " << g.maxX << "]\n";
  for (int z = g.minZ; z <= g.maxZ; ++z) {
    strm << "z=" << z << "\n";
    for (int y = g.minY; y <= g.maxY; ++y) {
      for (int x = g.minX; x <= g.maxX; ++x) {
        strm << (g.space[z][y][x] ? "#" : ".");
      }
      strm << "\n";
    }
    strm << "\n";
  }
  return strm;
}

// returns at most 4
int activeNeighbors(const Grid& g, int z, int y, int x) {
  int count = 0;
  for (int k = z-1; k <= z+1; ++k) {
    for (int j = y-1; j <= y+1; ++j) {
      for (int i = x-1; i <= x+1; ++i) {
        if (i == x && j == y && z == k) continue;
        if (g.space[k][j][i]) {
          count++;
          if (count > 3) return count;
        }
      }
    }
  }
  return count;
}

Grid step(const Grid& g) {
  Grid out;
  out.minX = g.minX-1;
  out.maxX = g.maxX+1;
  out.minY = g.minY-1;
  out.maxY = g.maxY+1;
  out.minZ = g.minZ-1;
  out.maxZ = g.maxZ+1;

  for (int z = out.minZ; z <= out.maxZ; ++z) {
    for (int y = out.minY; y <= out.maxY; ++y) {
      for (int x = out.minX; x <= out.maxX; ++x) {
        int count = activeNeighbors(g, z, y, x);
        if (g.space[z][y][x] && count == 2 || count == 3) {
          out.space[z][y][x] = true;
        } else if (count == 3 && !g.space[z][y][x]) {
          out.space[z][y][x] = true;
        }
      }
    }
  }

  return out;
}

uint32_t countActive(const Grid& g) {
  uint32_t count = 0;
  for (int z = g.minZ; z <= g.maxZ; ++z) {
    for (int y = g.minY; y <= g.maxY; ++y) {
      count += g.space[z][y].count();
    }
  }
  return count;
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  Grid g;

  string line;
  int j = 0;
  while(getline(inFile, line)) {
    const int dim = line.length();
    int y = (maxDim - dim)/2 + j;
    for (size_t i = 0; i < dim; ++i) {
      int x = (maxDim - dim)/2 + i;
      if (line[i] == '#') {
        g.space[15][y][x] = true;
      }
    }
    j++;
  }
  g.minZ = g.maxZ = 15;
  g.minX = g.minY = (maxDim - j)/2;
  g.maxX = g.maxY = (maxDim - j)/2 + j-1;
  cout << "Step 0:\n" << g;

  for (int i = 1; i <= 6; ++i) {
    Grid next = step(g);
    cout << "Step " << i << " has " << countActive(next) << " active:\n" << next;
    g = next;
  }

  return 0;
}
