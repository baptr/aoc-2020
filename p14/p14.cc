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

vector<uint64_t> floatAddrs(uint64_t baseAddr, uint64_t mask, int scanBit) {
  if (mask >> scanBit == 0) {
    return {baseAddr};
  }
  uint64_t check = uint64_t(1) << scanBit;
  if (mask & check) {
    vector<uint64_t> low = floatAddrs(baseAddr, mask, scanBit+1);
    vector<uint64_t> high = floatAddrs(baseAddr|check, mask, scanBit+1);
    low.insert(low.end(), high.cbegin(), high.cend());
    return low;
  } else {
    // TODO: could iterate this in place instead of recursing.
    return floatAddrs(baseAddr, mask, scanBit+1);
  }
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  uint64_t preserve = 0; // bits set to 1 where Xs are present in the mask
  uint64_t force = 0; // bits set to 1 where 1s are present in the mask
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
#if 0 // v1
      uint64_t val = (rawVal & preserve) | force;
      cout << "Setting mem @ " << addr << " = " << val << " (from " << rawVal << ")\n";
      mem[addr] = val;
#else // v2
      uint64_t baseAddr = (addr|force) & ~preserve;
      vector<uint64_t> addrs = floatAddrs(baseAddr, preserve, 0);
      for (const uint64_t a : addrs) {
        cout << "mem[" << bitset<38>(a) << "] = " << rawVal << "\n";
        mem[a] = rawVal;
      }
#endif
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
