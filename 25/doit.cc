// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>

using namespace std;

int xform(int a, int loop_sz) {
  int result = 1;
  for (int _ = 0; _ < loop_sz; ++_)
    result = long(result) * a % 20201227;
  return result;
}

int find_loop_sz(int target) {
  int xformed = 1;
  int loop_sz = 0;
  while (xformed != target) {
    xformed = long(xformed) * 7 % 20201227;
    ++loop_sz;
  }
  return loop_sz;
}

void part1() {
  int card_public, door_public;
  cin >> card_public >> door_public;
  int card_loop_sz = find_loop_sz(card_public);
  cout << xform(door_public, card_loop_sz) << '\n';
}

void part2() {
  cout << "Pay the Deposit!\n";
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
