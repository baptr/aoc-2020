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

bool shorter(set<string> *a, set<string> *b) {
  return a->size() < b->size();
}

int main(int argc, char** argv) {
  ifstream inFile;
  inFile.open("input.txt");

  map<string,string> decoder; // allergen -> ingredient (XXX or the other way?)
  // Find all dishes which list a given allergen.
  // Find the intersection of their ingredients.
  // If size = 1, record and remove that ingredient from all dishes.
  map<string,vector<set<string>*>> foods;
  vector<set<string>*> remnants;
  string tok;
  bool readAllergens = false;
  auto *ingredients = new set<string>;
  vector<string> allergens;
  while(inFile >> tok ) {
    if (readAllergens) {
      const char end = tok.back();
      tok.pop_back();
      allergens.push_back(tok);
      if (end == ')') { // Done
        //sort(ingredients->begin(), ingredients->end());
        remnants.push_back(ingredients);
        for (const string& a : allergens) {
          foods[a].push_back(ingredients);
        }
        ingredients = new set<string>;
        allergens.clear();
        readAllergens = false;
        continue;
      }
    } else if (tok == "(contains") {
      readAllergens = true;
      continue;
    } else {
      ingredients->emplace(tok);
    }
  }
  inFile.close();

  for (auto mapIt = foods.begin(); mapIt != foods.end();) {
    const string& a = mapIt->first;
    const vector<set<string>*> sets = mapIt->second;
    //sort(sets.begin(), sets.end(), shorter);
    set<string> common; 
    for (set<string>* it : sets) {
      if (common.empty()) {
        common.insert(it->begin(), it->end());
      } else {
        set<string> next;
        set_intersection(common.begin(), common.end(), it->begin(), it->end(), inserter(next, next.begin()));
        common = next;
      }
    }
    cout << "Allergen '" << a << "' is in " << sets.size() << " dishes, with " << common.size() << " possibilities\n";
    if (common.size() == 0) {
      cout << "Something messed up for " << a << "\n";
      exit(1);
    }
    if (common.size() == 1) {
      string answer = *common.begin();
      cout << "Solved " << a << " == " << answer << "\n";
      for (set<string>* it : remnants) {
        it->erase(answer);
      }
      foods.erase(a);
      // Restart iteration since we've simplified the solution.
      mapIt = foods.begin();
      continue;
    }
    mapIt++;
  }

  uint64_t total = 0;
  for (set<string>* it : remnants) {
    total += it->size();
  }
  cout << "Total remaining ingredients: " << total << "\n";

  return 0;
}
