// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cassert>

using namespace std;

struct food {
  set<string> ingredients;
  set<string> allergens;

  food(string const &s);

  bool contains(string const &thing) const {
    return ingredients.count(thing) || allergens.count(thing);
  }
};

food::food(string const &s) {
  stringstream ss(s);
  string ingredient;
  while (ss >> ingredient) {
    if (ingredient == "(contains")
      break;
    ingredients.insert(ingredient);
  }
  string allergen;
  while (ss >> allergen) {
    assert(allergen.back() == ',' || allergen.back() == ')');
    allergen.pop_back();
    allergens.insert(allergen);
  }
}

vector<food> foods;
set<string> allergens;
set<string> ingredients;

void read() {
  string line;
  while (getline(cin, line))
    foods.emplace_back(line);
  for (auto const &f: foods) {
    ingredients.insert(f.ingredients.begin(), f.ingredients.end());
    allergens.insert(f.allergens.begin(), f.allergens.end());
  }
}

set<string> intersection(set<string> const &s1, set<string> const &s2) {
  set<string> result;
  set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                   inserter(result, result.end()));
  return result;
}

set<string> difference(set<string> const &s1, set<string> const &s2) {
  set<string> result;
  set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
                 inserter(result, result.end()));
  return result;
}

pair<set<string>, map<string, set<string>>> allergen_possibilities() {
  read();
  set<string> dangerous;
  map<string, set<string>> possibilities;
  for (auto const &allergen : allergens) {
    auto could_be = ingredients;
    // The allergen could be anything in the intersection of the sets
    // of ingredients of all the foods known to contain that allergen
    for (auto const &f : foods)
      if (f.contains(allergen))
        could_be = intersection(could_be, f.ingredients);
    // Whatever ingredients the allergen could be are dangerous
    dangerous.insert(could_be.begin(), could_be.end());
    possibilities.emplace(allergen, could_be);
  }
  assert(dangerous.size() == allergens.size());
  // Everything known not to be dangerous is safe
  auto safe = difference(ingredients, dangerous);
  return { safe, possibilities };
}

void part1() {
  auto [safe, _] = allergen_possibilities();
  int ans = 0;
  for (auto const &f : foods)
    for (auto const &ingredient : safe)
      if (f.contains(ingredient))
        ++ans;
  cout << ans << '\n';
}

void part2() {
  auto [_, possibilities] = allergen_possibilities();
  // allergen => the ingredient containing it
  map<string, string> dangerous;
  while (!possibilities.empty()) {
    // Find an allergen that has only one possible ingredient
    optional<string> determined;
    for (auto const & [allergen, could_be] : possibilities)
      if (could_be.size() == 1) {
        dangerous.emplace(allergen, *could_be.begin());
        determined = allergen;
        break;
      }
    assert(determined);
    auto const &known = dangerous[*determined];
    // Remove that allergen and the now-known ingredient from the list
    // of possibilities
    possibilities.erase(*determined);
    for (auto & [_, could_be] : possibilities)
      could_be.erase(known);
  }
  bool first = true;
  for (auto const &[_, ingredient] : dangerous) {
    if (!first)
      cout << ',';
    first = false;
    cout << ingredient;
  }
  cout << '\n';
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "usage: " << argv[0] << " partnum < input\n";
    exit(1);
  }
  if (*argv[1] == '1')
    part1();
  else
    part2();
  return 0;
}
