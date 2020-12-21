#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

int accum = 0;

class Op {
public:
  string code;
  int val;
  bool seen = false;
  Op() {};
  Op(string c, int v) : code(c), val(v) {};
  string dbg() {
    ostringstream out;
    out << code << " " << val;
    return out.str();
  }
  virtual int step() = 0;
};

class NOP : public Op {
public:
  NOP(int v) : Op("NOP", v) {};
  int step() { return 1; }
};

class ACC : public Op {
public:
  ACC(int v) : Op("ACC", v) {};
  int step() {
    accum += val;
    return 1;
  }
};

class JMP : public Op {
public:
  JMP(int v) : Op("JMP", v) {};
  int step() {
    return val;
  }
};

Op* flipOP(Op* old) {
  if (old->code == "NOP") {
    return new JMP(old->val);
  } else if (old->code == "JMP") {
    return new NOP(old->val);
  } else {
    return old;
  }
}

bool eval(vector<Op*> prog, int* out) {
  accum = 0;
  int pc = 0;
  for (;;) {
    if (pc == prog.size()) {
      cout << "HALT at PC " << pc << " accumulator: " << accum << "\n";
      *out = accum;
      return true;
    }
    Op* o = prog[pc];
    if (o->seen) {
      cout << "Loop to PC " << pc << " accumulator: " << accum << "\n";
      *out = accum;
      return false;
    }
    cout << "PC " << pc << " ACC " << accum << " : " << o->dbg() << "\n";
    o->seen = true;
    pc += o->step();
  }
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  vector<Op*> prog;
  string line;
  while (getline(inFile, line)) {
    istringstream reader(line);
    string ins, val;
    reader >> ins >> val;

    int v = stoi(val);
    if (ins == "nop") {
      prog.push_back(new NOP(v));
    } else if (ins == "acc") {
      prog.push_back(new ACC(v));
    } else if (ins == "jmp") {
      prog.push_back(new JMP(v));
    }
  }
  inFile.close();

  int pc = 0;
  vector<int> flipable;
  for (;;) {
    Op* o = prog[pc];
    if (o->seen) {
      break;
    }
    if (o->code == "NOP" || o->code == "JMP") {
      flipable.push_back(pc);
    }
    o->seen = true;
    pc += o->step();
  }
  cout << "Base (looping) run accumulator: " << accum << "\n";

  for (auto flip = flipable.cbegin(); flip != flipable.end(); ++flip) {
    for (auto c = prog.begin(); c != prog.end(); ++c) {
      (*c)->seen = false;
    }
    if (flip > flipable.cbegin()) { // unflip the last attempt
      prog[*(flip-1)] = flipOP(prog[*(flip-1)]);
    }
    prog[*flip] = flipOP(prog[*flip]);
    accum = 0;
    pc = 0;
    int flipStep = 0;
    for (;;) {
      if (pc == prog.size()) {
        cout << "Flip PC=" << *flip << ": HALT at PC=" << pc << " accum: " << accum << "\n";
        return 0;
      }
      if (pc < 0 || pc > prog.size()) {
        cout << "Flip PC=" << *flip << ": OOB at PC=" << pc << " accum: " << accum << "\n";
        break;
      }
      Op* o = prog[pc];
      if (o->seen) {
        cout << "Flip PC=" << *flip << ": LOOP at PC=" << pc << " accum: " << accum << "\n";
        break;
      }
      o->seen = true;
      pc += o->step();
    }
  }

  return 0;
}
