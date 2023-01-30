// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

struct forest {
  vector<string> trees;

  forest();

  char at(unsigned rt, unsigned dn) const;

  int check(unsigned step_rt, unsigned step_dn) const;
};

forest::forest() {
  string row;
  while (getline(cin, row)) {
    trees.push_back(row);
    assert(row.length() == trees.front().length());
  }
  assert(!trees.empty());
}

char forest::at(unsigned rt, unsigned dn) const {
  assert(dn < trees.size());
  return trees[dn][rt % trees[dn].length()];
}

int forest::check(unsigned step_rt, unsigned step_dn) const {
  int result = 0;
  unsigned rt = 0;
  unsigned dn = 0;
  while (dn < trees.size()) {
    result += at(rt, dn) == '#';
    rt += step_rt;
    dn += step_dn;
  }
  return result;
}

void part1() { cout << forest().check(3, 1) << '\n'; }

void part2() {
  forest woody;
  int ans = woody.check(1, 2);
  for (int rt = 1; rt <= 7; rt += 2)
    ans *= woody.check(rt, 1);
  cout << ans << '\n';
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
