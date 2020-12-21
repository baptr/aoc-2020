#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;


int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  int total = 0;
  int allAgreed = 0;
  int groupCount = 0;
  int groupSize = 0;
  map<char, int> group;
  while (!inFile.eof()) {
    string line;
    getline(inFile, line);
    if (line.empty()) { // End of entry
      groupCount++;
      total += group.size();
      for (auto it = group.cbegin(); it != group.end(); ++it) {
        if (it->second == groupSize) {
          allAgreed++;
        }
      }
      group.clear();
      groupSize = 0;
      continue;
    }
    for (auto it = line.cbegin(); it != line.end(); ++it) {
      group[*it]++;
    }
    groupSize++;
  }
  inFile.close();

  cout << "Total (part 1): " << total << "\n";
  cout << "Agreed (part 2): " << allAgreed << "\n";
  cout << "Groups Processed: " << groupCount << "\n";

  return 0;
}
