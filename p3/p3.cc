#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

const int rightSlope = 3;

unsigned long countTrees(int r, int d) {
  ifstream inFile;
  inFile.open("input.txt");

  string line;
  int pos = 0;
  int count = 0;
  while (inFile >> line) {
    if (line[pos] == '#') {
      count++;
    }
    pos = (pos + r) % line.size();
    for (int down = 1; down < d; down++) {
      inFile >> line;
    }
  }

  inFile.close();
  return count;
}

int main(int argc, char** argv) {
  unsigned long base = countTrees(3, 1);
  cout << "3,1 hit: " << base << "\n";

  unsigned long a = countTrees(1, 1);
  unsigned long c = countTrees(5, 1);
  unsigned long d = countTrees(7, 1);
  unsigned long e = countTrees(1, 2);

  cout << "1,1: " << a << " 5,1: " << c << " 7,1: " << d << " 1,2: " << e << "\n";
  cout << "product: " << a * base * c * d * e << "\n";

  return 0;
}
