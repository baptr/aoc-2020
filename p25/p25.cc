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

uint64_t xfrm(uint64_t subj, int size) {
  uint64_t val = 1;
  for (int i = 0; i < size; ++i) {
    val = (val * subj) % 20201227;
  }
  return val;
}

int main(int argc, char** argv) {
  int found = 0;
  uint64_t val = 1;
  for (int i = 1; i < 10000000000; ++i) {
    val = (val * 7) % 20201227;
    // cout << "Size " << i << " = " << val << "\n";
    if (val == 10441485) {
      cout << "Size " << i << " = " << val << "\n";
      found++;
      cout << "Key1: " << xfrm(1004920, i) << "\n";
      if (found == 2) break;
    } else if (val == 1004920) {
      cout << "Size " << i << " = " << val << "\n";
      found++;
      cout << "Key2: " << xfrm(10441485, i) << "\n";
      if (found == 2) break;
    }
  }

  return 0;
}
