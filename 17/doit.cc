// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <set>

using namespace std;

using coords = vector<int>;

coords operator+(coords const &c1, coords const &c2) {
  coords result = c1;
  for (size_t i = 0; i < c1.size(); ++i)
    result[i] += c2[i];
  return result;
}

// The active cubes start out symmetric in all dimensions beyond the
// first two and that symmetry will be preserved.  So for dimensions
// 3, 4, ..., only store the first "quadrant".  It would be possible
// to take advantage of higher symmetries for dimension 4 (w and z are
// swappable, so could, e.g., canonicalize with z >= w).  That's a bit
// more awkward though, and given only 4 dimensions it's probably not
// worth the complication.

struct cubes {
  // What's active in the first "quadrant"
  set<coords> active;
  // For scanning the around a cube (includes the all-0 offset too)
  vector<coords> deltas;

  // Construct from stdin
  cubes(unsigned dim);

  bool at(coords c) const;

  // Active neighbors (excludes c if that's active)
  int active_neighbors(coords const &c) const;

  // One step of the simulation
  void step();

  // How many active cells there are (taking into account the
  // first-quadrant business)
  int num_active() const;
};

cubes::cubes(unsigned dim) {
  int y = 0;
  string row;
  while (getline(cin, row)) {
    int x = 0;
    for (char ch : row) {
      coords c{ x, y };
      while (c.size() < dim)
        c.push_back(0);
      if (ch == '#')
        active.insert(c);
      ++x;
    }
    ++y;
  }
  deltas = { { -1 }, { 0 }, { +1 } };
  while (deltas.front().size() < dim) {
    vector<coords> next_deltas;
    for (int d = -1; d <= +1; ++d)
      for (auto delta : deltas) {
        delta.push_back(d);
        next_deltas.push_back(delta);
      }
    deltas = next_deltas;
  }
}

bool cubes::at(coords c) const {
  // Dimensions beyond the first two are symmetric about 0
  for (size_t i = 2; i < c.size(); ++i)
    c[i] = abs(c[i]);
  return active.count(c);
}

int cubes::active_neighbors(coords const &c) const {
  int result = at(c) ? -1 : 0;
  for (auto const &delta : deltas)
    if (at(c + delta))
      ++result;
  return result;
}

void cubes::step() {
  set<coords> next_active;
  set<coords> checked;
  for (auto const &c : active)
    // New cells can only appear within one unit of an active cell
    for (auto const &delta : deltas) {
      coords c1 = c + delta;
      // Only consider the first quadrant
      for (size_t i = 2; i < c1.size(); ++i)
        c1[i] = abs(c1[i]);
      if (!checked.insert(c1).second)
        // Already considered this location
        continue;
      int nbrs = active_neighbors(c1);
      if (nbrs == 3 || (nbrs == 2 && at(c1)))
        next_active.insert(c1);
    }
  active = next_active;
}

int cubes::num_active() const {
  int result = 0;
  for (auto const &c : active) {
    int num_copies = 1;
    for (size_t i = 2; i < c.size(); ++i)
      if (c[i] > 0)
        num_copies *= 2;
    result += num_copies;
  }
  return result;
}

void solve(unsigned dim) {
  cubes c(dim);
  for (int _ = 0; _ < 6; ++_)
    c.step();
  cout << c.num_active() << '\n';
}

void part1() { solve(3); }
void part2() { solve(4); }

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
