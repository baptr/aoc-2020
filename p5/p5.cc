#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

int index(string val) {
  int out = 0;
  for (auto it = val.cbegin(); it != val.end(); ++it) {
    out <<= 1;
    if (*it == 'B' || *it == 'R') {
      out |= 1;
    }
  }
  return out;
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  string line;
  int max = -1;
  vector<int> found;
  while (inFile >> line) {
    int idx = index(line);
    found.push_back(idx);
    cout << line << " = " << idx << "\n";
    if (idx > max) max = idx;
  }
  inFile.close();

  sort(found.begin(), found.end());
  for (auto it = found.cbegin(); it+1 != found.end(); ++it) {
    if (*it + 1 != *(it+1)) {
      cout << "Missing seat idx: " << *it + 1 << "\n";
    }
  }

  cout << "Max Seat Index: " << max << "\n";

  return 0;
}
