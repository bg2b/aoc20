// -*- C++ -*-
// Part 2 is a bit less than 10 seconds; optimization will get it
// under 3, so maybe worthwhile
// g++ -std=c++17 -Wall -g -O -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <list>
#include <map>
#include <algorithm>
#include <cassert>

using namespace std;

list<int> play(unsigned extras, int steps) {
  string order;
  cin >> order;
  list<int> cups;
  for (char c : order)
    cups.push_back(c - '0');
  while (cups.size() < extras)
    cups.push_back(cups.size() + 1);
  int max_cup = cups.size();
  // The current cup will always be kept at the head of cups.  The
  // only issue is finding where to move cups, i.e., going from
  // destination number to the location in the list.  We'll just keep
  // a vector loc of iterators, with loc[i - 1] being the iterator
  // pointing to i.
  vector<list<int>::iterator> loc(max_cup);
  auto location = [&](int i) -> list<int>::iterator & { return loc[i - 1]; };
  for (auto i = cups.begin(); i != cups.end(); ++i)
    location(*i) = i;
  assert(*location(max_cup) == max_cup);
  for (int _ = 0; _ < steps; ++_) {
    // Pick up cups clockwise from the current one
    int const num_moved = 3;
    auto move_start = next(cups.begin());
    auto move_end = move_start;
    advance(move_end, num_moved);
    list<int> to_move;
    to_move.splice(to_move.begin(), cups, move_start, move_end);
    // Invalidate iterators of items being moved so the scan below
    // will skip them
    location(to_move.front()) = cups.end();
    location(*next(to_move.begin())) = cups.end();
    location(to_move.back()) = cups.end();
    // Find where to move the cups
    auto wrap = [=](int cup) { return cup < 1 ? max_cup : cup; };
    int dest = wrap(cups.front() - 1);
    list<int>::iterator desti;
    while ((desti = location(dest)) == cups.end())
      dest = wrap(dest - 1);
    assert(*desti == dest);
    // Insert the moved items back into cups
    cups.splice(next(desti), to_move, to_move.begin(), to_move.end());
    // Update iterators of those items
    for (int i = 0; i < num_moved; ++i) {
      ++desti;
      location(*desti) = desti;
    }
    // Rotate clockwise
    cups.splice(cups.end(), cups, cups.begin(), next(cups.begin()));
  }
  // Move 1 to the end of the list
  if (cups.back() != 1)
    rotate(cups.begin(), next(location(1)), cups.end());
  return cups;
}

void part1() {
  auto cups = play(0, 100);
  cups.pop_back();
  for (auto cup : cups)
    cout << cup;
  cout << '\n';
}

void part2() {
  auto cups = play(1000000, 10000000);
  cout << long(cups.front()) * *next(cups.begin()) << '\n';
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
