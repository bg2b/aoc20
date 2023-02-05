// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>

using namespace std;

// Hexes identified with combos of basis vectors for east = (1, 0) and
// northeast = (0, 1)

using coords = pair<int, int>;

coords operator+(coords const &c1, coords const &c2) {
  return { c1.first + c2.first, c1.second + c2.second };
}

map<string, coords> dirs{
  { "e", { +1, 0 } },
  { "ne", { 0, +1 } },
  { "nw", { -1, +1 } },
  { "w", { -1, 0 } },
  { "sw", { 0, -1 } },
  { "se", { +1, -1 } }
};

struct tiles {
  // Coordinates of black tiles
  set<coords> black;

  // Construct from stdin
  tiles();

  // Flip a tile identified by the directions in s
  void flip(string const &s);

  // Flip tiles for the next day
  void day();

  size_t num_black() const { return black.size(); }
};

tiles::tiles() {
  string line;
  while (cin >> line)
    flip(line);
}

void tiles::flip(string const &s) {
  coords pos{ 0, 0 };
  for (size_t i = 0; i < s.length(); ) {
    string dir(1, s[i]);
    if (dirs.find(dir) == dirs.end()) {
      assert(i + 1 < s.length());
      dir.push_back(s[i + 1]);
    }
    auto p = dirs.find(dir);
    assert(p != dirs.end());
    pos = pos + p->second;
    i += dir.length();
  }
  if (black.count(pos))
    black.erase(pos);
  else
    black.insert(pos);
}

void tiles::day() {
  set<coords> considered;
  set<coords> next_black;
  // Consider a tile and decide whether it should be black or white on
  // the next day
  auto consider =
    [&](coords const &c) {
      if (considered.count(c))
        // Already done
        return;
      considered.insert(c);
      int black_nbrs = 0;
      for (auto const & [_, dir] : dirs)
        if (black.count(c + dir))
          ++black_nbrs;
      // Tiles with 2 black neighbors are always black.  Tiles with 1
      // black neighbor remain black if they already are.
      if (black_nbrs == 2 || (black_nbrs == 1 && black.count(c)))
        next_black.insert(c);
    };
  // Consider black tiles and tiles adjacent to them
  for (auto const &c : black) {
    consider(c);
    for (auto const &[_, dir] : dirs)
      consider(c + dir);
  }
  black = next_black;
}

void part1() { cout << tiles().num_black() << '\n'; }

void part2() {
  tiles floor;
  for (int _ = 0; _ < 100; ++_)
    floor.day();
  cout << floor.num_black() << '\n';
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
