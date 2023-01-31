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
  vector<int> adapters;
  int adapter;
  while (cin >> adapter)
    adapters.push_back(adapter);
  sort(adapters.begin(), adapters.end());
  return adapters;
}

void part1() {
  auto adapters = read();
  size_t diff1 = 0;
  size_t diff3 = 0;
  // Start with 0 at the outlet
  int last_jolts = 0;
  for (size_t i = 0; i < adapters.size(); ++i) {
    int diff = adapters[i] - last_jolts;
    assert(1 <= diff && diff <= 3);
    if (diff == 1)
      ++diff1;
    else if (diff == 3)
      ++diff3;
    last_jolts = adapters[i];
  }
  // Final 3 to the device
  ++diff3;
  cout << diff1 * diff3 << '\n';
}

void part2() {
  auto adapters = read();
  // It's convenient to have the outlet explicit to avoid an edge case
  adapters.insert(adapters.begin(), 0);
  vector<size_t> ways(1, 1);
  for (size_t i = 1; i < adapters.size(); ++i) {
    ways.push_back(0);
    for (size_t j = i; j-- > 0 && adapters[i] - adapters[j] <= 3; )
      // Adapter i can connect directly to anything within 3 jolts
      ways[i] += ways[j];
  }
  cout << ways.back() << '\n';
  ways.clear();
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
