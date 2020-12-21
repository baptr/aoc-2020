#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

struct Rule {
  int count;
  string type;
};

map<string, vector<string>> heldBy;
map<string, bool> seen;
map<string, vector<Rule>> rules;

void search(string bag) {
  auto holder = heldBy.find(bag);
  if (holder == heldBy.end()) {
    cout << "Bag '" << bag << "' is not held by any others\n";
    return;
  }
  vector<string> &step = holder->second;
  for (auto it = step.cbegin(); it != step.end(); ++it) {
    if (seen[*it]) {
      continue;
    }
    search(*it);
    seen[*it] = true;
  }
  return;
}

int fill(string bag) {
  int count = 0;
  vector<Rule> r = rules[bag];
  for (auto it = r.cbegin(); it != r.end(); ++it) {
    count += it->count * (1 + fill(it->type));
  }
  return count;
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  string line;
  while (getline(inFile, line)) {
    istringstream reader(line);
    string adj, color, skip;
    reader >> adj >> color >> skip >> skip;

    string sub = adj + " " + color;
    // # adj color bags?[.,]
    while (!reader.eof()) {
      string count;
      reader >> count >> adj >> color >> skip;
      if (count == "no") {
        continue;
      }
      int num = stoi(count);
      string obj = adj + " " + color;
      cout << "'" << sub << "' holds " << num << "x '" << obj << "'\n";
      heldBy[obj].push_back(sub);
      rules[sub].push_back(Rule{num, obj});
    }
  }
  inFile.close();

  search("shiny gold");
  cout << "Golden holders: " << seen.size() << "\n";

  int total = fill("shiny gold");
  cout << "Golden fill: " << total << "\n";


  return 0;
}
