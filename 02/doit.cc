// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

using namespace std;

void check_valid(function<bool(int, int, char, string const &)> valid) {
  int ans = 0;
  unsigned low, high;
  char letter, _;
  string passwd;
  while (cin >> low >> _ >> high >> letter >> _ >> passwd)
    if (valid(low, high, letter, passwd))
      ++ans;
  cout << ans << '\n';
}

void part1() {
  check_valid([](int low, int high, char c, string const &passwd) {
                int uses = count(passwd.begin(), passwd.end(), c);
                return low <= uses && uses <= high;
              });
}

void part2() {
  check_valid([](int low, int high, char c, string const &passwd) {
                return (passwd[low - 1] == c) != (passwd[high - 1] == c);
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
