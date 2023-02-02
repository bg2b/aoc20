// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct init_prog {
  // X's in most recent mask
  long x_mask{0};
  // 0's and 1' in most recent mask
  long set_mask{0};
  // For version 2, the individual bits from x_mask
  vector<long> x_bits;
  // Memory contents
  map<long, long> mem;

  // Read from stdin, fill memory
  init_prog(bool version2);

  // Sum of memory contents
  long mem_sum() const;
};

init_prog::init_prog(bool version2) {
  string what, val;
  char equals;
  while (cin >> what >> equals >> val) {
    if (what == "mask") {
      x_mask = 0;
      set_mask = 0;
      for (char c : val) {
        x_mask <<= 1;
        set_mask <<= 1;
        if (c == 'X')
          x_mask |= 1;
        else if (c == '1')
          set_mask |= 1;
      }
      if (version2) {
        // Decompose x_mask into individual bits
        x_bits.clear();
        for (long x = x_mask; x != 0; ) {
          // Isolate lowest set bit
          long bit = ((x ^ (x - 1)) + 1) >> 1;
          x_bits.push_back(bit);
          // Peel off that one
          x ^= bit;
        }
      }
    } else  {
      long addr = stol(what.substr(4));
      long v = stol(val);
      if (!version2) {
        // Version 1, tweak the data
        mem[addr] = (x_mask & v) | set_mask;
        continue;
      }
      // Version 2, run through all possible addresses.  This is only
      // reasonable since the number of X's in each mask are limited
      // (at most 9 in my input).  If masks were like in the first
      // example shown in part 1, an implicit representation would be
      // required.
      size_t num_addr = 1 << x_bits.size();
      addr = (~x_mask & addr) | set_mask;
      for (size_t i = 0; i < num_addr; ++i) {
        long addr1 = addr;
        for (size_t j = 0; j < x_bits.size(); ++j)
          if (i & (1 << j))
            addr1 |= x_bits[j];
        mem[addr1] = v;
      }
    }
  }
}

long init_prog::mem_sum() const {
  long result = 0;
  for (auto [_, v] : mem)
    result += v;
  return result;
}

void part1() { cout << init_prog(false).mem_sum() << '\n'; }
void part2() { cout << init_prog(true).mem_sum() << '\n'; }

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
