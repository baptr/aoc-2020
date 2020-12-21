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

  return 0;
}
