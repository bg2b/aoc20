// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

void decls(function<string(string const &, string const &)> update) {
  int result = 0;
  bool new_group = true;
  string group;
  string person;
  while (getline(cin, person)) {
    if (person.empty()) {
      result += group.length();
      group.clear();
      new_group = true;
      continue;
    }
    sort(person.begin(), person.end());
    person.erase(unique(person.begin(), person.end()), person.end());
    if (new_group)
      group = person;
    else
      group = update(group, person);
    new_group = false;
  }
  result += group.length();
  cout << result << '\n';
}

void part1() {
  decls([](string const &s1, string const &s2) {
          string result;
          set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),
                    back_inserter(result));
          return result;
        });
}

void part2() {
  decls([](string const &s1, string const &s2) {
          string result;
          set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                           back_inserter(result));
          return result;
        });
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
