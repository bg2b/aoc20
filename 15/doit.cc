// -*- C++ -*-
// Optimization definitely needed for part 2, ~15 seconds even then.
// I don't know if there's some clever observation that I'm missing
// that lets part 2 run in reasonable time.
//
// g++ -std=c++17 -Wall -g -O -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

vector<int> read() {
  string line;
  getline(cin, line);
  line.push_back(',');
  stringstream ss(line);
  int num;
  char comma;
  vector<int> nums;
  while (ss >> num >> comma) {
    assert(comma == ',');
    nums.push_back(num);
  }
  return nums;
}

void play(size_t turns) {
  auto starting = read();
  map<int, pair<int, int>> spoken_on;
  int last_spoken;
  for (size_t turn = 0; turn < turns; ++turn) {
    if (turn < starting.size())
      // Speak a starting number
      last_spoken = starting[turn];
    else {
      auto p = spoken_on.find(last_spoken);
      assert(p != spoken_on.end());
      // Speak the age
      last_spoken = p->second.first - p->second.second;
    }
    auto p = spoken_on.find(last_spoken);
    if (p == spoken_on.end())
      // First time spoken, "age" above will be 0
      spoken_on.emplace(last_spoken, make_pair(turn, turn));
    else {
      // Remember this turn and the previous one for computing age
      p->second.second = p->second.first;
      p->second.first = turn;
    }
  }
  cout << last_spoken << '\n';
}

void part1() { play(2020); }
void part2() { play(30000000); }

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
