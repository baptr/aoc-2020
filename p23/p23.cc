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

int chooseDest(int size, int cur, const vector<int>& held) {
  for (int o = 1; o < 5; ++o) {
    int chk = (size-1+cur-o)%size+1;
    bool ok = true;
    for (const int i : held) {
      if (chk == i) {
        ok = false;
        break;
      }
    }
    if (ok) {
      return chk;
    }
  }
  cout << "CHOOSE DEST FAILED\n";
  return -1;
}

void dump(const vector<int>& ring, int focus) {
  const int size = ring.size();
  int prev = -1;
  bool elided = false;
  for (int idx = 0; idx < size; ++idx) {
    if (idx == focus) {
      cout << "(" << ring[idx] << ") ";
      prev = ring[idx];
      continue;
    }
    int v = ring[idx];
    if (prev+1 == v && idx+1 < size && v+1 == ring[idx+1]) {
      if (!elided) {
        cout << ".";
        elided = true;
      }
      prev = v;
      continue;
    }
    prev = v;
    if (elided) {
      cout << ". ";
      elided = false;
    }
    cout << v << " ";
  }
  cout << "\n";
}

void shift(vector<int>& ring, int focus, int destIdx, vector<int>& index) {
  const int size = ring.size();
  for (int k = 1; k <= (size+destIdx-(focus+3))%size; ++k) {
    int to = (focus+k)%size;
    int from = (focus+3+k)%size;
    // cout << "moving ring[" << from << "] = " << ring[from] << " to ring[" << to << "]\n";
    ring[to] = ring[from];
    index[ring[to]] = to;
  }
}

void replaceHeld(vector<int>& ring, vector<int>& held, int destIdx, vector<int>& index) {
  const int size = ring.size();
  for (int l = 1; l < 4; ++l) {
    int from = l-1;
    int to = (size+destIdx-3+l)%size;
    //cout << "injecting held=" << held[from] << " at ring[" << to << "]\n";
    ring[to] = held[from];
    index[ring[to]] = to;
  }
}

int main(int argc, char** argv) {
  if (argc < 3) {
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

  if (argc == 4) {
    int target = stoi(argv[3]);
    cout << "Phase 2 - padding to " << target << "... ";
    for (int i = ring.size()+1; i <= target; ++i) {
      ring.push_back(i);
    }
  }

  const int size = ring.size();
  cout << "ring.size=" << size << "\n";
  vector<int> index(size);
  for (int i = 0; i < size; ++i) {
    index[ring[i]] = i;
  }
  vector<int> hold;
  for (int move = 0; move < count; ++move) {
    int focus = move % size;
    int cur = ring[focus];
    for (int i = 1; i < 4; ++i) {
      hold.push_back(ring[(focus+i)%size]);
    }

    int destVal = chooseDest(size, cur, hold);
    int destIdx = index[destVal];
    if (count <= 1000 || move % 1000 == 0) {
      cout << "Move " << (move+1) << " focus: ring[" << focus << "]=" << cur << " dest: ring[" << destIdx << "]=" << destVal << " (=" << ring[destIdx] << ")\n";
      if (size <= 1000 || move % 50000 == 0) {
        dump(ring, focus);
      }
    }
    
    // now shift focus+4 .. destIdx down to focus+1
    shift(ring, focus, destIdx, index);
    // then add the held
    replaceHeld(ring, hold, destIdx, index);
    hold.clear();
  }

  cout << "\n";

  if (size < 10) { // Phase 1 (lazy)
    cout << "Final order:\n";
    for (const int i : ring) {
      cout << i;
    }
    cout << "\n";
  }
  for (auto it = ring.cbegin(); it != ring.cend(); ++it) {
    // XXX this might need to loop
    if (*it == 1) {
      ++it;
      uint64_t first = *it;
      ++it;
      uint64_t second = *it;
      cout << "After 1: " << first << " " << second << "\n";
      cout << "Answer: " << (first * second) << "\n";
      break;
    }
  }

  return 0;
}
