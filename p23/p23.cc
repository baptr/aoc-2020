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

struct Node {
  int val;
  Node* next;
};

int chooseDest(int size, const Node* focus) {
  const int cur = focus->val;
  const Node* hold = focus->next;
  for (int o = 1; o < 5; ++o) {
    int chk = (size-1+cur-o)%size+1;
    bool ok = true;
    Node* h = (Node*)hold;
    for (int j = 0; j < 3; ++j) {
      if (chk == h->val) {
        ok = false;
        break;
      }
      h = h->next;
    }
    if (ok) {
      return chk;
    }
  }
  cout << "CHOOSE DEST FAILED\n";
  return -1;
}

void dump(const Node* focus) {
  cout << "(" << focus->val << ") ";
  int prev = -1;
  bool elided = false;
  for (Node* n = focus->next; n != focus; n = n->next) {
    const int v = n->val;
    if (prev+1 == v && v+1 == n->next->val) {
      if (!elided) {
        cout << ".";
        elided = true;
      }
    } else {
      if (elided) {
        cout << ". ";
        elided = false;
      }
      cout << v << " ";
    }
    prev = v;
  }
  cout << "\n";
}

void shift(Node* focus, Node* dest) {
  // Move focus+1, focus+2, focus+3 after dest
  Node* holdHead = focus->next; // focus+1
  Node* holdEnd = holdHead->next->next; // focus+3
  Node* nextFocus = holdEnd->next; // focus+4
  Node* destNext = dest->next;
  dest->next = holdHead;
  holdEnd->next = destNext;
  focus->next = nextFocus;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cout << "Need initial label string and iteration count\n";
    exit(1);
  }

  int labels = stoi(argv[1]);
  int count = stoi(argv[2]);

  cout << "Initial labels: " << labels << " iteration count: " << count << "\n";

  Node* focus = NULL;
  Node* writeHead = NULL;
  int size = 0;
  for (const char* c = argv[1]; *c != 0; ++c) {
    Node* n = new Node();
    n->val = *c - '0';
    if (writeHead == NULL) {
      focus = n;
      writeHead = n;
    } else {
      writeHead->next = n;
      writeHead = n;
    }
    size++;
  }

  if (argc == 4) {
    int target = stoi(argv[3]);
    cout << "Phase 2 - padding to " << target << "... ";
    for (int i = size+1; i <= target; ++i) {
      Node* n = new Node();
      n->val = i;
      writeHead->next = n;
      writeHead = n;
      size++;
    }
  }
  writeHead->next = focus; // Loop it

  cout << "ring.size=" << size << "\n";
  map<int,Node*> index;
  for (Node* n = focus;; n = n->next) {
    index[n->val] = n;
    if (n->next == focus) break;
  }

  Node* holdHead;
  for (int move = 0; move < count; ++move) {
    int cur = focus->val;

    int destVal = chooseDest(size, focus);
    Node* destIdx = index[destVal];
    if (count <= 1000 || move % 100000 == 0) {
      cout << "Move " << (move+1) << " focus: ring[" << focus << "]=" << cur << " dest: ring[" << destIdx << "]=" << destVal << ")\n";
      if (size <= 1000 || move % 5000000 == 0) {
        dump(focus);
      }
    }
    
    // now shift focus+4 .. destIdx down to focus+1
    shift(focus, destIdx);
    focus = focus->next;
  }

  cout << "\n";

  if (size < 10) { // Phase 1 (lazy)
    cout << "Final order:\n";
    for (const Node* n = focus;; n = n->next) {
      cout << n->val;
      if (n->next == focus) break;
    }
    cout << "\n";
  }
  for (const Node* n = focus;; n = n->next) {
    if (n->val == 1) {
      n = n->next;
      uint64_t first = n->val;
      n = n->next;
      uint64_t second = n->val;
      cout << "After 1: " << first << " " << second << "\n";
      cout << "Answer: " << (first * second) << "\n";
      break;
    }
    if (n->next == focus) {
      cout << "MISSED 1\n";
      break;
    }
  }

  return 0;
}
