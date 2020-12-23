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

using namespace std;


int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  string startTime, busList;
  inFile >> startTime;
  inFile >> busList;
  inFile.close();

  map<unsigned int, int> buses; // number -> index offset
  vector<unsigned int> busSpeed;
  size_t start = 0;
  for (int index = 0;; ++index) {
    size_t end = busList.find(",", start);
    string bus = busList.substr(start, end-start);
    if (bus != "x") {
      unsigned long num = stoul(bus);
      buses.emplace(num, index);
      busSpeed.push_back(num);
    }
    if (end == string::npos) break;
    start = end+1;
  }
  sort(busSpeed.begin(), busSpeed.end());
  reverse(busSpeed.begin(), busSpeed.end());

  unsigned int time = stoul(startTime);
  unsigned int minWait = UINT_MAX;
  unsigned int firstBus = 0;
  for (auto it = buses.cbegin(); it != buses.cend(); ++it) {
    unsigned int bus = it->first;
    unsigned int sinceDeparture = time % bus;
    unsigned int wait = bus - sinceDeparture;
    cout << "bus " << bus << " left " << sinceDeparture << " ago, next in " << wait << "\n";
    if (wait < minWait) {
      minWait = wait;
      firstBus = bus;
    }
  }

  cout << "min wait: " << minWait << " minutes for bus: " << firstBus << " : answer: " << minWait * firstBus << "\n";

  // I suspect there's still 1 step further I could go/some underlying
  // principal that I'm just guessing at here, but here goes..
  // Once we find the first alignment for a subset of buses, we can step from
  // there by the multiple of each of those, and ~sieve out from there.
  uint64_t base = 0;
  uint64_t step = 1;
  for (unsigned int & bus : busSpeed) {
    int index = buses[bus];
    for (; (base+index) % bus != 0; base += step) {}
    step *= bus;
    cout << "After bus " << bus << " base=" << base << " step=" << step << "\n";
  }
  cout << "Base: " << base << "\n";

  for (const auto &kv : buses) {
    unsigned int bus = kv.first;
    int index = kv.second;
    uint64_t hit = (base+index) % bus;
    cout << "Check: bus " << bus << " at " << base+index << " miss by " << hit << "\n";
  }

  return 0;
}
