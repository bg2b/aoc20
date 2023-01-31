// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cassert>

using namespace std;

int const n = 25;

using num = long;

struct XMAS {
  // All the numbers
  vector<num> seq;
  // The sliding window
  list<num> window;
  // Sums of pairs of numbers in the window => number of occurrences
  // of the sum
  map<num, int> sums;

  // Read preamble from stdin
  XMAS();

  // Add next number, return whether it's valid
  bool check_valid(num x);

  // Find encryption weakness
  num weakness(num x) const;
};

XMAS::XMAS() {
  for (int i = 0; i < n; ++i) {
    num x;
    cin >> x;
    assert(cin);
    seq.push_back(x);
    window.push_back(x);
  }
  for (auto i = window.begin(); i != window.end(); ++i)
    for (auto j = next(i); j != window.end(); ++j)
      ++sums[*i + *j];
}

bool XMAS::check_valid(num x) {
  if (!sums.count(x))
    // Invalid
    return false;
  seq.push_back(x);
  // Drop start of window
  num head = window.front();
  for (auto i = next(window.begin()); i != window.end(); ++i) {
    auto p = sums.find(head + *i);
    assert(p != sums.end());
    if (--p->second == 0)
      sums.erase(p);
  }
  window.pop_front();
  // Add x to window
  for (auto i = window.begin(); i != window.end(); ++i)
    ++sums[*i + x];
  window.push_back(x);
  return true;
}

num XMAS::weakness(num x) const {
  size_t start = 0;
  size_t end = 0;
  num sum = 0;
  while (sum != x)
    if (sum < x) {
      // Needs to be bigger, advance end and include that
      assert(end + 1 < seq.size());
      sum += seq[++end];
    } else
      // Needs to be smaller, advance start and drop that
      sum -= seq[++start];
  auto [min, max] = minmax_element(&seq[start], &seq[end]);
  return *min + *max;
}

void solve(bool show_weakness) {
  XMAS xmas;
  num x;
  do {
    cin >> x;
    assert(cin);
  } while (xmas.check_valid(x));
  cout << (show_weakness ? xmas.weakness(x) : x) << '\n';
}

void part1() { solve(false); }
void part2() { solve(true); }

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
