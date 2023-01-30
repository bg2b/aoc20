// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

vector<int> read() {
  vector<int> result;
  string pass;
  while (cin >> pass) {
    assert(pass.length() == 10);
    int row = 0;
    for (int i = 0; i < 7; ++i)
      row = 2 * row + (pass[i] == 'F' ? 0 : 1);
    int col = 0;
    for (int i = 0; i < 3; ++i)
      col = 2 * col + (pass[i + 7] == 'L' ? 0 : 1);
    result.push_back(8 * row + col);
  }
  return result;
}

void part1() {
  auto ids = read();
  cout << *max_element(ids.begin(), ids.end()) << '\n';
}

void part2() {
  auto ids = read();
  sort(ids.begin(), ids.end());
  for (size_t i = 0; i + 1 < ids.size(); ++i)
    if (ids[i] + 2 == ids[i + 1])
      cout << ids[i] + 1 << '\n';
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
