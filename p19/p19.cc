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
// #include <utility>

using namespace std;

struct RuleDef {
  enum Tag{LEAF, SPLIT, SEQ};
  Tag tag;

  union {
    char leaf;
    pair<RuleDef*, RuleDef*> split;
    vector<int> seq;
  };

  RuleDef(string s) {
    if (s == "\"a\"") {
      tag = LEAF;
      leaf = 'a';
    } else if (s == "\"b\"") {
      tag = LEAF;
      leaf = 'b';
    } else {
      size_t sPos = s.find("|");
      if (sPos != string::npos) {
        tag = SPLIT;
        split = make_pair(new RuleDef(s.substr(0, sPos-1)), new RuleDef(s.substr(sPos+1)));
      } else {
        tag = SEQ;
        istringstream str(s);
        string tok;
        while(str >> tok) {
          seq.push_back(stoi(tok));
        }
      }
    }
  }
  ~RuleDef() {}
};

ostream& operator<<(ostream& strm, const RuleDef& r) {
  if (r.tag == r.LEAF) {
    return strm << r.leaf;
  } else if (r.tag == r.SPLIT) {
    return strm << r.split.first << " | " << r.split.second;
  } else if (r.tag == r.SEQ) {
    for (int idx : r.seq) {
      strm << idx << " ";
    }
    return strm;
  }
  return strm << "untagged def";
}

string build(map<int, RuleDef*> rules, RuleDef* r) {
  if (r->tag == r->LEAF) {
    return string(1, r->leaf);
  }
  if (r->tag == r->SPLIT) {
    return "(" + build(rules, r->split.first) + "|" + build(rules, r->split.second) + ")";
  }
  if (r->tag == r->SEQ) {
    ostringstream out;
    for (int i : r->seq) {
      out << build(rules, rules.find(i)->second);
    }
    return out.str();
  }
  return "<bad def!>";
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  map<int, RuleDef*> rules;
  string line;
  while(getline(inFile, line)) {
    if (line.empty()) break;
    int id = stoi(line);
    size_t pos = line.find(" ")+ 1;
    rules.emplace(id, new RuleDef(line.substr(pos)));
  }
  // Build match tree..
  // Process input..
  inFile.close();

  cout << "42: " << build(rules, rules.find(42)->second) << "\n";
  cout << "\n";
  cout << "31: " << build(rules, rules.find(31)->second) << "\n";

  return 0;
}
