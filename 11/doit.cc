// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

using coords = pair<int, int>;

coords operator+(coords const &c1, coords const &c2) {
  return { c1.first + c2.first, c1.second + c2.second };
}

coords operator*(int sc, coords const &c) {
  return { sc * c.first, sc * c.second };
}

vector<coords> dirs{
  { -1, -1 }, { 0, -1 }, { 1, -1 },
  { -1,  0 },            { 1,  0 },
  { -1,  1 }, { 0,  1 }, { 1,  1 },
};

struct waiting_area {
  // State of the seats
  vector<string> seats;
  // For each seat, which are the seats to count as neighbors
  map<coords, vector<coords>> around;

  // Construct from stdin; if adjacent_only, use only immediately
  // adjacent seats for around, else scan all the way to the edge
  waiting_area(bool adjacent_only);

  bool in_bounds(coords const &c) const;
  char at(coords const &c) const { return seats[c.second][c.first]; }

  // Do one round of the simulation, with threshold being the
  // tolerance for people leaving due to overcrowding
  bool round(int threshold);

  int num_occupied() const;
};

waiting_area::waiting_area(bool adjacent_only) {
  string row;
  while (getline(cin, row)) {
    seats.push_back(row);
    assert(row.length() == seats.front().length());
  }
  assert(!seats.empty());
  // Maximum distance to scan for a neighboring seat
  int max_dist = 1;
  if (!adjacent_only)
    max_dist = max(seats.size(), seats.front().length());
  // Find neighbors
  for (size_t y = 0; y < seats.size(); ++y)
    for (size_t x = 0; x < seats[y].length(); ++x) {
      coords c{ x, y };
      if (at(c) != 'L')
        // Not a seat
        continue;
      auto &nbrs = around[c];
      for (auto const &dir : dirs)
        for (int dist = 1; dist <= max_dist; ++dist) {
          coords c1 = c + dist * dir;
          if (!in_bounds(c1))
            // Off the edge
            break;
          if (at(c1) == 'L') {
            // Found the neighbor in this direction
            nbrs.push_back(c1);
            break;
          }
        }
    }
}

bool waiting_area::in_bounds(coords const &c) const {
  return (c.second >= 0 && c.second < int(seats.size()) &&
          c.first >= 0 && c.first < int(seats[c.second].length()));
}

bool waiting_area::round(int threshold) {
  auto next = seats;
  for (auto const & [c, nbrs] : around) {
    int occupied_nbrs = 0;
    for (auto const &nbr : nbrs)
      if (at(nbr) == '#')
        ++occupied_nbrs;
    char ch = at(c);
    if (ch == '#' && occupied_nbrs >= threshold)
      ch = 'L';
    else if (ch == 'L' && occupied_nbrs == 0)
      ch = '#';
    next[c.second][c.first] = ch;
  }
  bool stable = seats == next;
  seats = next;
  return stable;
}

int waiting_area::num_occupied() const {
  int result = 0;
  for (size_t y = 0; y < seats.size(); ++y)
    for (size_t x = 0; x < seats[y].length(); ++x)
      if (at({ x, y }) == '#')
        ++result;
  return result;
}

void solve(bool part1) {
  waiting_area seats(part1);
  while (!seats.round(part1 ? 4 : 5))
    ;
  cout << seats.num_occupied() << '\n';
}

void part1() { solve(true); }
void part2() { solve(false); }

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
