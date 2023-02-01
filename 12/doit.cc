// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <cassert>

using namespace std;

using coords = pair<int, int>;

coords operator+(coords const &c1, coords const &c2) {
  return { c1.first + c2.first, c1.second + c2.second };
}

coords operator*(int sc, coords const &c) {
  return { sc * c.first, sc * c.second };
}

coords left(coords const &c) { return { -c.second, c.first }; }
coords right(coords const &c) { return { c.second, -c.first }; }

void solve(bool part1) {
  // Ship position
  coords pos{ 0, 0 };
  // Waypoint position
  coords dir;
  if (part1)
    dir = coords{ 1, 0 };
  else
    dir = coords{ 10, 1 };
  // What's being moved by N, S, E, W?
  auto &to_move = part1 ? pos : dir;
  char cmd;
  unsigned amount;
  while (cin >> cmd >> amount) {
    switch (cmd) {
    case 'L':
      assert(amount % 90 == 0);
      for (unsigned turn = 0; turn < amount; turn += 90)
        dir = left(dir);
      break;
    case 'R':
      assert(amount % 90 == 0);
      for (unsigned turn = 0; turn < amount; turn += 90)
        dir = right(dir);
      break;
    case 'F':
      pos = pos + amount * dir;
      break;
    case 'N':
      to_move = to_move + amount * coords{ 0, +1 };
      break;
    case 'S':
      to_move = to_move + amount * coords{ 0, -1 };
      break;
    case 'E':
      to_move = to_move + amount * coords{ +1, 0 };
      break;
    default:
      assert(cmd == 'W');
      to_move = to_move + amount * coords{ -1, 0 };
      break;
    }
  }
  cout << abs(pos.first) + abs(pos.second) << '\n';
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
