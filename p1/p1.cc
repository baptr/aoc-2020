#include <iostream>
#include <fstream>
#include <map>

using namespace std;

const int target = 2020;

int main(int argc, char** argv) {
  std::map<int,bool> inputs;

  ifstream inFile;
  inFile.open("input.txt");
  int in;
  while (inFile >> in) {
    inputs.emplace(in, true);
  }
  cout << "Read " << inputs.size() << '\n';

  for (auto it = inputs.cbegin(); it != inputs.end(); ++it) {
    const int val = it->first;
    const int need = target - val;
    if (inputs.find(need) != inputs.end()) {
      cout << val << " + " << need << " = " << target << " : " << val * need << "\n";
    }
  }

  std::map<int,int> firstSum;
  for (auto it = inputs.cbegin(); it != inputs.end(); ++it) {
    const int val1 = it->first;
    for (auto it2 = inputs.cbegin(); it2 != inputs.end(); ++it2) {
      if (it == it2) continue;
      const int val2 = it2->first;
      firstSum.emplace(val1+val2, val1*val2);
    }
  }
  for (auto it = inputs.cbegin(); it != inputs.end(); ++it) {
    const int val = it->first;
    const int need = target - val;
    auto found = firstSum.find(need);
    if (found != firstSum.end()) {
      cout << "3-sum mult: " << found->second * val << "\n";
    }
  }


  return 0;
}
