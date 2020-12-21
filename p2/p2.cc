#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

const int target = 2020;

bool check(const string password, const char key, int min, int max) {
  int count = 0;
  for (auto it = password.cbegin(); it != password.end(); ++it) {
    if (*it == key) {
      count++;
    }
  }
  return min <= count && count <= max;
}

bool check2(const string password, const char key, int first, int second) {
  if (second > password.size()) return false;
  bool firstHas = password[first-1] == key;
  bool secondHas = password[second-1] == key;
  return firstHas ^ secondHas;
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");
  string range, target, password;
  int count1 = 0;
  int count2 = 0;
  while (inFile >> range >> target >> password) {
    string::size_type pos;
    pos = range.find("-"); // TODO check
    int min, max;
    min = stoul(range.substr(0, pos), 0, 10);
    max = stoul(range.substr(pos+1), 0, 10);

    if (check(password, target[0], min, max)) {
      count1++;
    }
    if (check2(password, target[0], min, max)) {
      count2++;
    }
  }
  cout << "First count: " << count1 << "\n";
  cout << "Second count: " << count2 << "\n";

  return 0;
}
