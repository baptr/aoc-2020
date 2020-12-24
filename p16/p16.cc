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

struct Interval {
  int low, high;
  Interval(string s) {
    size_t pos = s.find("-");
    low = stoi(s.substr(0, pos));
    high = stoi(s.substr(pos+1));
  }
};

struct Rule {
  string name;
  Interval a, b;
  bool Allows(int val) const {
    return (a.low <= val && val <= a.high) ||
      (b.low <= val && val <= b.high);
  }
};

ostream& operator<<(ostream& strm, const Rule& r) {
  return strm << "field[" << r.name << "] = [" << r.a.low << " to " << r.a.high << "] and [" << r.b.low << " to " << r.b.high << "]";
}

struct Ticket {
  vector<int> vals;
  Ticket(string s) {
    size_t start = 0;
    for (;;) {
      size_t end = s.find(",", start);
      vals.push_back(stoi(s.substr(start, end)));
      if (end == string::npos) break;
      start = end+1;
    }
  }
};

ostream& operator<<(ostream& strm, const Ticket& t) {
  for (const int& i : t.vals) {
    strm << i << ", ";
  }
  return strm;
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  // [field name]: [low]-[high] or [low]-[high]
  // ...
  //
  // your ticket:
  // [val],[val],...
  //
  // nearby tickets:
  // [val],[val],...
  string line;
  vector<Rule> rules;
  while(getline(inFile, line)) {
    if (line.empty()) break;
    size_t splitPos = line.find(":");
    string name = line.substr(0, splitPos);
    string vals = line.substr(splitPos+2);
    splitPos = vals.find(" or ");
    Interval a(vals.substr(0, splitPos));
    Interval b(vals.substr(splitPos+4));
    rules.push_back({name, a, b});
    cout << rules.back() << "\n";
  }
  Ticket* myTicket;
  while(getline(inFile, line)) {
    if (line.empty()) break;
    if (line == "your ticket:") continue;
    myTicket = new Ticket(line);
  }
  cout << "Mine: " << *myTicket << "\n";
  vector<Ticket> others;
  while(getline(inFile, line)) {
    if (line.empty()) break;
    if (line == "nearby tickets:") continue;
    others.push_back({line});
    cout << "Other #" << others.size() << " : " << others.back() << "\n";
  }
  cout << "Loaded " << others.size() << " nearby tickets\n";
  inFile.close();

  int badTotal = 0;
  vector<Ticket> valid = {*myTicket};
  for (const Ticket& t : others) {
    bool tBad = false;
    for (int v : t.vals) {
      bool ok = false;
      for (const Rule& r : rules) {
        if (r.Allows(v)) {
          ok = true;
          break;
        }
      }
      if (!ok) {
        cout << "val " << v << " is bad\n";
        badTotal += v;
        tBad = true;
      }
    }
    if (!tBad) {
      valid.push_back(t);
    }
  }
  cout << "badTotal: " << badTotal << "\n";

  map<string, list<int>> matches;
  for (const Rule& r : rules) {
    for (int field=0; field < rules.size(); ++field) {
      bool ok = true;
      for (const Ticket& t : valid) {
        if (!r.Allows(t.vals[field])) {
          ok = false;
          break;
        }
      }
      if (ok) {
        cout << r << " matches field index " << field << "\n";
        matches[r.name].push_back(field);
      }
    }
  }

  map<string, int> assignment;
  while (!matches.empty()) {
    for (auto aIt = matches.begin(); aIt != matches.end(); ) {
      const string& name = aIt->first;
      list<int>& m = aIt->second;
      if (m.size() > 1) {
        ++aIt;
        continue;
      }
      int pick = m.front();
      assignment[name] = pick;
      cout << "Assigned " << name << " = #" << pick << "\n";
      aIt = matches.erase(aIt);
      for (auto& [n, l] : matches) {
        for (auto it = l.begin(); it != l.end();) {
          if (*it == pick) {
            it = l.erase(it);
          } else {
            ++it;
          }
        }
      }
    }
  }

  unsigned long answer = 1;
  for (const auto [name, field] : assignment) {
    if (name.find("departure ") == string::npos) continue;
    answer *= myTicket->vals[field];
  }
  cout << "Departure multiple: " << answer << "\n";

  return 0;
}
