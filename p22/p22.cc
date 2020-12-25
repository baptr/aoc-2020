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

  int round;
  for (round = 1; player1.size() != 0 && player2.size() != 0; ++round) {
    int a = player1.front(); player1.pop_front();
    int b = player2.front(); player2.pop_front();
    if (a > b) {
      player1.push_back(a);
      player1.push_back(b);
    } else {
      player2.push_back(b);
      player2.push_back(a);
    }
  }
  cout << "After round " << round << "..\n";

  cout << "player 1: ";
  dumpHand(player1);
  cout << " score = " << score(player1) << "\n";
  cout << "player 2: ";
  dumpHand(player2);
  cout << " score = " << score(player2) << "\n";


  return 0;
}
