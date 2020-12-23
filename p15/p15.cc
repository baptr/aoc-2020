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

  string tok;
  int idx = 1;
  map<int,int> spoken;
  int last;
  int next = 0; 
  while(inFile >> tok) {
    int val = stoi(tok);
    spoken[val] = idx;
    cout << "[" << idx << "] speaks " << val << "\n";
    idx++;
    last = val;
  }
  inFile.close();

  for (; idx <= 2020; ++idx) {
    last = next;
    if (spoken.find(next) == spoken.end()) { // new
      next = 0;
    } else { // repeat
      next = idx - spoken[next];
    }
    spoken[last] = idx;
    cout << "[" << idx << "] speaks " << last << "\n";
  }

  return 0;
}
