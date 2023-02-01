// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include <numeric>
#include <cctype>
#include <cassert>

using namespace std;

int depart;
vector<long> ids;
vector<int> offsets;

void read() {
  cin >> depart;
  string buses;
  cin >> buses;
  buses.push_back(',');
  stringstream ss(buses);
  int offset = 0;
  while (ss.peek(), !ss.eof()) {
    char comma;
    if (isdigit(ss.peek())) {
      int id;
      ss >> id >> comma;
      ids.push_back(id);
      offsets.push_back(offset);
    } else {
      char x;
      ss >> x >> comma;
      assert(x == 'x');
    }
    ++offset;
    assert(comma == ',');
  }
  assert(!ids.empty());
  assert(ids.size() == offsets.size());
}

void part1() {
  read();
  optional<int> min_wait;
  int best_id = ids.front();
  for (auto id : ids) {
    int this_wait;
    if (depart % id == 0)
      this_wait = 0;
    else
      this_wait = id - depart % id;
    if (!min_wait || this_wait < *min_wait) {
      min_wait = this_wait;
      best_id = id;
    }
  }
  assert(min_wait);
  cout << best_id * *min_wait << '\n';
}

// A group of buses, with schedules first appropriately aligned at
// time .first, and with a period of .second.
using fleet = pair<long, long>;

// Align two fleets (simple sieving method)
fleet align(fleet const &f1, fleet const &f2) {
  auto [t1, period1] = f1;
  auto [t2, period2] = f2;
  if (period1 < period2) {
    // Main loop strides with the bigger period
    swap(period1, period2);
    swap(t1, t2);
  }
  // Wait for initial alignment
  auto t = max(t1, t2);
  // Align with period1
  if ((t - t1) % period1 != 0)
    t += period1 - (t - t1) % period1;
  // Align with period2
  while ((t - t2) % period2 != 0)
    t += period1;
  return { t, lcm(period1, period2) };
}

void part2() {
  read();
  auto fleet1 = [](size_t bus) {
                  // A fleet of one bus
                  int id = ids[bus];
                  int offset = offsets[bus];
                  return fleet{ offset == 0 ? id : id - offset, id };
                };
  auto f = fleet1(0);
  for (size_t i = 1; i < ids.size(); ++i)
    f = align(f, fleet1(i));
  cout << f.first << '\n';
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
