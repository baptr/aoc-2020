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

int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "Need initial label string and iteration count\n";
    exit(1);
  }

  int labels = stoi(argv[1]);
  int count = stoi(argv[2]);

  cout << "Initial labels: " << labels << " iteration count: " << count << "\n";

  vector<int> ring;
  for (const char* c = argv[1]; *c != 0; ++c) {
    ring.push_back(*c - '0');
  }

  const int size = ring.size();
  vector<int> hold;
  for (int move = 0; move < count; ++move) {
    int focus = move % size;
    cout << "Move " << (move+1) << " focus=" << focus << "\n";
    int cur = ring[focus];
    for (int i = 1; i < 4; ++i) {
      hold.push_back(ring[(focus+i)%size]);
    }
    int destIdx;
    int score = 0;
    for (int j = 4; j < size; ++j) {
      int idx = (focus + j) % size;
      int val = ring[idx];
      int d = (size + val - cur) % size;
      // cout << "ring[" << idx << "] = " << val << ", " << d << " >? " << score << "\n";
      if (d > score) {
        destIdx = idx;
        score = d;
      }
    }
    // cout << "ring[" << destIdx << "] = " << ring[destIdx] << " target after src ring[" << focus << "] = " << cur << "\n";
    
    // now shift focus+4 .. destIdx down to focus+1
    // then add the held
    for (int k = 1; k <= (size+destIdx-(focus+3))%size; ++k) {
      int to = (focus+k)%size;
      int from = (focus+3+k)%size;
      //cout << "moving ring[" << from << "] = " << ring[from] << " to ring[" << to << "]\n";
      ring[to] = ring[from];
    }
    for (int l = 1; l < 4; ++l) {
      int from = l-1;
      int to = (size+destIdx-3+l)%size;
      //cout << "injecting held=" << hold[from] << " at ring[" << to << "]\n";
      ring[to] = hold[from];
    }
    hold.clear();
  }

  cout << "Final order:\n";
  for (const int i : ring) {
    cout << i;
  }
  cout << "\n";

  return 0;
}
