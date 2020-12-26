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

typedef int OP;
const OP kNone = 0;
const OP kAdd = 1;
const OP kMult = 2;

uint64_t calc(const string& str, size_t start = 0, size_t* end = NULL) {
  // "1 + 1 + 1"  = 3
  // "1 + (1 + 1)" = 1 + calc("1 + 1")
  // "(1 + 1) + 1" = calc("1 + 1") + 1
  // "(1 + 1)" = calc("1 + 1")
  // "((1 + 1) + 1)" = calc("(1 + 1) + 1") = calc("1 + 1") + 1
  //
  // part2:
  // "1 + 1 * 1 + 1 * 1 + 1" = 2 * calc("1 + 1 * 1 + 1") = 2 * (2 * calc("1 + 1")) = 2 * (2 * 2) = 8
  uint64_t val = 0;
  OP pending = kNone;
  size_t pos;
  for (pos = start; pos < str.length(); ++pos) {
    //cout << "Processing " << str << "[" << start << ", " << pos << "] = " << str[pos] << "; val = " << val << "\n";
    if (str[pos] == ' ') continue;
    if (str[pos] == ')') {
      break;
    }
    if (str[pos] == '(') {
      uint64_t subVal = calc(str, pos+1, &pos);
      if (pending == kNone) {
        val = subVal;
      } else if (pending == kAdd) {
        val += subVal;
      }
      continue;
    }
    if (str[pos] == '+') {
      pending = kAdd;
      continue;
    }
    if (str[pos] == '*') {
      uint64_t subVal = calc(str, pos+2, &pos);
      val *= subVal;
      pos--;
      continue;
    }
    size_t eon = str.find_first_not_of("1234567890", pos);
    uint64_t subVal = stoi(str.substr(pos, eon));
    if (pending == kNone) {
      val = subVal;
    } else if (pending == kAdd) {
      val += subVal;
    }
    pos = eon-1;
  }
  if (end != NULL) {
    *end = pos;
  }
  //cout << "Returning: " << val << " end pos: " << pos << "\n";
  return val;
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  string line;
  uint64_t total = 0;
  while(getline(inFile, line)) {
    uint64_t val = calc(line);
    cout << line << " = " << val << "\n";
    total += val;
  }
  inFile.close();

  cout << "Total: " << total << "\n";

  return 0;
}
