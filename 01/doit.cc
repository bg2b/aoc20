// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

vector<int> report;

void read() {
  int item;
  while (cin >> item)
    report.push_back(item);
  sort(report.begin(), report.end());
  assert(!report.empty());
}

void part1() {
  read();
  size_t j = report.size();
  for (size_t i = 0; i < j; ++i) {
    while (j - 1 > i && report[i] + report[j - 1] > 2020)
      --j;
    if (report[i] + report[j - 1] == 2020) {
      cout << report[i] * report[j - 1] << '\n';
      break;
    }
  }
}

void part2() {
  read();
  size_t j = report.size();
  for (size_t i = 0; i < j; ++i) {
    // The two minimum values are at i and i + 1
    while (j - 2 > i && report[i] + report[i + 1] + report[j - 1] > 2020)
      --j;
    // Scan up from i + 1 while too small
    size_t k = i + 1;
    while (k + 1 < j && report[i] + report[k] + report[j - 1] < 2020)
      ++k;
    if (k + 1 < j && report[i] + report[k] + report[j - 1] == 2020) {
      cout << report[i] * report[k] * report[j - 1] << '\n';
      break;
    }
  }
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
