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

void dumpHand(list<int> hand) {
  for (const int c : hand) {
    cout << c << " ";
  }
}

uint64_t score(list<int> hand) {
  uint64_t out = 0;
  uint32_t mult = hand.size();
  for (const int c : hand) {
    out += mult * c;
    mult--;
  }
  return out;
}

// part 1 solution
void play1(list<int> p1, list<int> p2) {
  int round;
  for (round = 1; p1.size() != 0 && p2.size() != 0; ++round) {
    int a = p1.front(); p1.pop_front();
    int b = p2.front(); p2.pop_front();
    if (a > b) {
      p1.push_back(a);
      p1.push_back(b);
    } else {
      p2.push_back(b);
      p2.push_back(a);
    }
  }
  cout << "After round " << round << "..\n";

  cout << "player 1: ";
  dumpHand(p1);
  cout << " score = " << score(p1) << "\n";
  cout << "player 2: ";
  dumpHand(p2);
  cout << " score = " << score(p2) << "\n";
}

list<int> subList(const list<int> in, size_t len) {
  list<int> out;
  for (int i : in) {
    out.push_back(i);
    len--;
    if (len == 0) break;
  }
  return out;
}

// true if p1 wins, false if p2 wins
bool play2(list<int> p1, list<int> p2, uint64_t* out_score = NULL) {
  set<pair<list<int>,list<int>>> seenStates;
  while (p1.size() != 0 && p2.size() != 0) {
    auto insrt = seenStates.emplace(make_pair(p1, p2));
    if (!insrt.second) {
      return true;
    }
    int a = p1.front(); p1.pop_front();
    int b = p2.front(); p2.pop_front();
    if (a <= p1.size() && b <= p2.size()) { // recurse
      if (play2(subList(p1, a), subList(p2, b))) {
        p1.push_back(a);
        p1.push_back(b);
      } else {
        p2.push_back(b);
        p2.push_back(a);
      }
    } else {
      if (a > b) {
        p1.push_back(a);
        p1.push_back(b);
      } else {
        p2.push_back(b);
        p2.push_back(a);
      }
    }
  }
  if (out_score != NULL) {
    *out_score = score(p1) + score(p2);
  }
  return p1.size() > 0;
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  string tok;
  inFile >> tok >> tok; // throw away player header
  list<int> player1;
  while(inFile >> tok ) {
    if (tok == "Player") {
      inFile >> tok; // number
      break;
    }
    player1.push_back(stoi(tok));
  }
  list<int> player2;
  while(inFile >> tok) {
    player2.push_back(stoi(tok));
  }
  inFile.close();

  /*
  cout << "Simple game:\n";
  play1(player1, player2);
  */

  uint64_t score = 0;
  bool p1Win = play2(player1, player2, &score);
  cout << "Recursive game p1_win=" << p1Win << " with score = " << score << "\n";

  return 0;
}
