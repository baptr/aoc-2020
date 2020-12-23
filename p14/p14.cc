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

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  uint64_t preserve = 0;
  uint64_t force = 0;
  map<uint64_t,uint64_t> mem;

  string line;
  while(getline(inFile, line)) {
    if (line.substr(0, 6) == "mask =") {
      string maskStr = line.substr(7);
      preserve = 0;
      force = 0;
      for (char c : maskStr) {
        preserve <<= 1;
        force <<= 1;
        if (c == 'X') {
          preserve |= 1;
        } else if (c == '1') {
          force |= 1;
        } else if (c == '0') {
          // force |= 0;
        } else {
          cout << "Unparsed mask field '" << c << "' in directive " << line << "\n";
        }
      }
    } else if (line.substr(0, 4) == "mem[") {
      uint64_t addr = stoul(line.substr(4));
      uint64_t rawVal = stoul(line.substr(line.find("=")+2));
      uint64_t val = (rawVal & preserve) | force;
      cout << "Setting mem @ " << addr << " = " << val << " (from " << rawVal << ")\n";
      mem[addr] = val;
    }
  }
  inFile.close();

  uint64_t total = 0;
  for (const auto &kv : mem) {
    total += kv.second;
  }

  cout << "Memory total: " << total << "\n";

  return 0;
}
