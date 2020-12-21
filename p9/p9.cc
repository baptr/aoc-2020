#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <list>
#include <set>

using namespace std;

#if 1
const size_t size = 25;
#else
const size_t size = 5;
#endif

bool check(const list<unsigned long> &hist, unsigned long val) {
  set<unsigned long> rem(hist.begin(), hist.end());
  for (auto it = hist.begin(); it != hist.end(); ++it) {
    int need = val - *it;
    if (rem.find(need) != rem.end()) {
      return true;
    }
  }
  return false;
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  string line;
  list<unsigned long> hist;
  // Load the preamble
  while (inFile >> line) {
    unsigned long v = stoul(line);
    hist.push_back(v);
    if (hist.size() == size) {
      break;
    }
  }
  unsigned long target;
  while (inFile >> line) {
    unsigned long v = stoul(line);
    bool ok = check(hist, v);
    if (!check(hist, v)) {
      cout << "Failed check: " << v << "\n";
      target = v;
      break;
    }
    hist.pop_front();
    hist.push_back(v);
  }
  inFile.close();

  cout << "Looking for weakness...\n";

  hist.clear();
  inFile.open("input.txt");
  unsigned long accum = 0;
  while (inFile >> line) {
    unsigned long v = stoul(line);
    accum += v;
    hist.push_back(v);
    while (accum > target) {
      accum -= hist.front();
      hist.pop_front();
    }
    if (accum == target) {
      unsigned long min = -1, max = 0;
      for (auto it = hist.cbegin(); it != hist.end(); ++it) {
        if (*it < min) min = *it;
        if (*it > max) max = *it;
      }
      cout << "Found weakness (len " << hist.size() << "): " << min << " + " << max << " = " << min+max << "\n";
      break;
    }
  }
  inFile.close();

  return 0;
}
