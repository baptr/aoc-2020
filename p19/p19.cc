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
#include <queue>

using namespace std;

struct RuleDef {
  enum Tag{LEAF, SPLIT, SEQ, RAW_SEQ, MATCH};
  Tag tag;

  union {
    char leaf;
    pair<RuleDef*, RuleDef*> split;
    vector<RuleDef*> seq;
    vector<int> raw_seq;
  };

  RuleDef(string s) {
    if (s == "<MATCH>") {
      tag = MATCH;
    } else if (s == "\"a\"") {
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
        tag = RAW_SEQ;
        istringstream str(s);
        string tok;
        while(str >> tok) {
          raw_seq.push_back(stoi(tok));
        }
      }
    }
  }
  ~RuleDef() {}
};

ostream& operator<<(ostream& strm, const RuleDef* r) {
  if (r->tag == r->LEAF) {
    return strm << r->leaf;
  } else if (r->tag == r->SPLIT) {
    return strm << r->split.first << " | " << r->split.second;
  } else if (r->tag == r->RAW_SEQ) {
    for (int idx : r->raw_seq) {
      strm << idx << " ";
    }
    return strm;
  } else if (r->tag == r->SEQ) {
    cout << "(";
    for (const RuleDef* c : r->seq) {
      strm << c << " ";
    }
    return strm << ")";
  } else if (r->tag == r->MATCH) {
    return strm << "<M>";
  }
  return strm << "untagged def";
}

string buildRE(const RuleDef* r) {
  if (r->tag == r->LEAF) {
    return string(1, r->leaf);
  } else if (r->tag == r->SPLIT) {
    return "(" + buildRE(r->split.first) + "|" + buildRE(r->split.second) + ")";
  } else if (r->tag == r->SEQ) {
    ostringstream out;
    for (const RuleDef* r : r->seq) {
      out << buildRE(r);
    }
    return out.str();
  } else if (r->tag == r->RAW_SEQ) {
    cout << "still a raw_seq: ";
    for (int i : r->raw_seq) {
      cout << i << " ";
    }
    cout << "\n";
  } else if (r->tag == r->MATCH) {
    return "";
  }
  return "<bad def!>";
}

struct State {
  const State* parent; // XXX ref count children?
  const int seq_idx; // if parent is a seq, this holds the index of this rule within it
  const RuleDef* rule;
  const size_t pos;
};

ostream& operator<<(ostream& strm, const State* s) {
  if (s == NULL) return strm << "<null>";
  return strm << "State(parent=" << s->parent << "[" << s->seq_idx << "], rule=" << s->rule << ", strpos=" << s->pos << ")";
}

bool match(const string& str, const RuleDef* root) {
  // 0: 1 2
  // 1: a b | b a
  // 2: 1 | 1 1
  // (2x or 3x "ab" or "ba")
  // abbaba
  // baba
  queue<State*> q;
  q.push(new State{NULL, 0, root, 0});
  while (!q.empty()) {
    State* s = q.front(); q.pop();
    // cout << s << "\n";
    const RuleDef *r = s->rule;
    if (s->pos >= str.length() && r->tag != r->MATCH) { // overshot
      // cout << "Overshot\n";
      delete s;
      continue;
    }
    if (r->tag == r->MATCH) { // successfully reached end of parent's sequence
      const State* p = s->parent;
      const State* g = p->parent;
      if (g == NULL) {
        if (s->pos == str.length()) return true; // success!
        // cout << "<M> Didn't use the whole input\n";
        // Match didn't use the whole input.
        delete s;
        continue;
      }
      // cout << "Match, pushing new check state\n";
      int next_idx = p->seq_idx+1;
      q.push(new State{g, next_idx, g->rule->seq[next_idx], s->pos});
      delete s;
    } else if (r->tag == r->SPLIT) {
      // No need to revisit splits, pass through to their parent.
      q.push(new State{s->parent, s->seq_idx, r->split.first, s->pos});
      q.push(new State{s->parent, s->seq_idx, r->split.second, s->pos});
      delete s;
    } else if (r->tag == r->SEQ) {
      // XXX could this handle seq continuation?
      q.push(new State{s, 0, r->seq[0], s->pos});
    } else if (r->tag == r->LEAF) {
      if (str[s->pos] != r->leaf) { // Non-match
        delete s;
        continue;
      }
      // leaf match, advance parents
      const State *p = s->parent;
      if (p == NULL) {
        if (s->pos+1 == str.length()) return true; // XXX cleanup
        // Match but didn't use the whole input.
        // cout << "<L> Didn't use the whole input\n";
        delete s;
        continue;
      }
      int next_idx = s->seq_idx+1;
      q.push(new State{p, next_idx, p->rule->seq[next_idx], s->pos+1});
      delete s;
    } else {
      cout << "Unrecognized rule tag " << r->tag << "\n";
      delete s;
    }
  }
  return false;
}

RuleDef* MATCH_RULE = new RuleDef("<MATCH>");

void expand(RuleDef* r, const map<int, RuleDef*> rules) {
  if (r->tag != r->RAW_SEQ) return;
  vector<RuleDef*> out;
  for (int i : r->raw_seq) {
    out.push_back(rules.find(i)->second);
  }
  out.push_back(MATCH_RULE);
  r->tag = r->SEQ;
  r->seq = out;
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
  // Flesh out the match tree...
  for (auto &[idx, r] : rules) {
    if (r->tag == r->RAW_SEQ) expand(r, rules);
    if (r->tag == r->SPLIT) {
      expand(r->split.first, rules);
      expand(r->split.second, rules);
    }
  }

  // Process input..
  int matches = 0;
  const RuleDef* r0 = rules.find(0)->second;
  while(getline(inFile, line)) {
    cout << "Processing " << line << " ... ";
    if (match(line, r0)) {
      cout << "Match!\n";
      matches++;
    } else {
      cout << " no match :-(\n";
    }
  }
  inFile.close();

  cout << "# matches: " << matches << "\n";

  // cout << "0: " << buildRE(r0) << "\n";
  /*
  cout << "42: " << buildRE(rules.find(42)->second) << "\n";
  cout << "\n";
  cout << "31: " << buildRE(rules.find(31)->second) << "\n";
  */

  return 0;
}
