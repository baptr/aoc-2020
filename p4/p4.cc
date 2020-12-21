#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Validator {
  public:
  virtual bool check(string val) = 0;
};

class Year : public Validator {
  int min;
  int max;

  public:
  Year(int min, int max) {
    this->min = min;
    this->max = max;
  }

  bool check(string val) {
    int v = stoi(val);
    return min <= v && v <= max;
  }
};

class Height : public Validator {
  public:
  bool check(string val) {
    size_t pos = 0;
    int v = stoi(val, &pos);
    string unit = val.substr(pos);
    if (unit == "in") {
      return 59 <= v && v <= 76;
    } else if (unit == "cm") {
      return 150 <= v && v <= 193;
    } else {
      return false;
    }
  }
};

class HexColor : public Validator {
  public:
  bool check(string val) {
    if (val.size() != 7) return false;
    if (val[0] != '#') return false;
    for (auto it = val.begin()+1; it != val.end(); ++it) {
      if ((*it >= 'a' && *it <= 'f') || (*it >= '0' && *it <= '9')) {
        continue;
      }
      return false;
    }

    return true;
  }
};

class OneOf : public Validator {
  map<string,bool> valid;

  public:
  OneOf(vector<string> opts) {
    for (auto it = opts.cbegin(); it != opts.end(); ++it) {
      valid.emplace(*it, true);
    }
  }

  bool check(string val) {
    return valid.find(val) != valid.end();
  }
};

class Number : public Validator {
  int digits;

  public:
  Number(int digits) {
    this->digits = digits;
  }

  bool check(string val) {
    if (val.size() != digits) return false;
    for (auto it = val.begin(); it != val.end(); ++it) {
      if (*it < '0' || *it > '9') return false;
    }
    return true;
  }
};

const map<string,Validator*> required {
  {"byr", new Year(1920, 2002)},
  {"iyr", new Year(2010, 2020)},
  {"eyr", new Year(2020, 2030)},
  {"hgt", new Height()},
  {"hcl", new HexColor()},
  {"ecl", new OneOf(vector<string>({"amb", "blu", "brn", "gry", "grn", "hzl", "oth"}))},
  {"pid", new Number(9)},
  // {"cid", false},
};

bool valid(const map<string,string> entry) {
  for (auto chk = required.cbegin(); chk != required.end(); ++chk) {
    auto got = entry.find(chk->first);
    if (got == entry.end()) {
      cout << "missing " << chk->first << "\n";
      return false;
    }
    if (!chk->second->check(got->second)) {
      cout << "failed validation: " << got->first << " bad val: '" << got->second << "'\n";
      return false;
    }
  }
  return true;
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  int count = 0;
  map<string,string> contents;
  while (!inFile.eof()) {
    string line;
    getline(inFile, line);
    if (line.empty()) { // End of entry
      if (valid(contents)) {
        count++;
      }
      contents.clear();
      continue;
    }
    for (;;) {
      size_t eor = line.find(' '); // maybe npos
      size_t kv = line.find(':');
      if (kv == string::npos) {
        cout << "Unable to find ':' in line: " << line << "\n";
        break;
      }
      string key = line.substr(0, kv);
      string val = line.substr(kv+1, eor-kv-1);
      contents.emplace(key, val);

      if (eor == string::npos) break;
      line = line.substr(eor+1);
    }
  }
  inFile.close();

  cout << "Valid: " << count << "\n";

  return 0;
}
