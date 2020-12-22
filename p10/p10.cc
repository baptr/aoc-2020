#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <list>
#include <set>
#include <algorithm>

using namespace std;

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  string line;
  vector<int> bag = {0};
  while (inFile >> line) {
    int v = stoi(line);
    bag.push_back(v);
  }
  inFile.close();

  sort(bag.begin(), bag.end());
  bag.push_back(bag.back()+3);

  map<int,int> gaps;
  for (auto it = bag.cbegin(); it != bag.end(); ++it) {
    if (it == bag.cbegin()) continue;
    int gap = *it - *(it-1);
    gaps[gap]++;
  }

  for (auto it = gaps.cbegin(); it != gaps.end(); ++it) {
    cout << "gap[" << it->first << "]: " << it->second << "\n";
  }
  cout << "gap[1] * gap[3] = " << gaps[1] * gaps[3] << "\n";

  map<int,unsigned long> paths = {{bag.back(), 1}};
  for (auto rit = bag.crbegin(); rit != bag.crend(); ++rit) {
    if (rit == bag.crbegin()) continue;
    unsigned long opts = 0;
    for (int i = *rit+1; i <= *rit+3; ++i) {
      auto hit = paths.find(i);
      if (hit == paths.end()) continue;
      opts += (unsigned long)hit->second;
    }
    paths.emplace(*rit, opts);
  }
  cout << "Paths from 0: " << paths[0] << "\n";

  return 0;
}
