// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

vector<pair<string, int>> read() {
  vector<pair<string, int>> prog;
  string instr;
  int arg;
  while (cin >> instr >> arg)
    prog.emplace_back(instr, arg);
  assert(!prog.empty());
  return prog;
}

pair<int, bool> exec(vector<pair<string, int>> const &prog) {
  int pc = 0;
  int accum = 0;
  vector<bool> seen(prog.size(), false);
  while (!seen[pc]) {
    seen[pc] = true;
    auto [instr, arg] = prog[pc];
    if (instr == "nop")
      ++pc;
    else if (instr == "acc") {
      accum += arg;
      ++pc;
    } else {
      assert(instr == "jmp");
      pc += arg;
    }
    if (pc == int(prog.size()))
      return { accum, true };
    assert(pc >= 0 && pc < int(prog.size()));
  }
  return { accum, false };
}

void part1() { cout << exec(read()).first << '\n'; }

void part2() {
  auto prog = read();
  for (auto & [instr, arg] : prog) {
    if (instr == "acc")
      continue;
    auto save = instr;
    instr = instr == "nop" ? "jmp" : "nop";
    auto [accum, halted] = exec(prog);
    if (halted) {
      cout << accum << '\n';
      return;
    }
    instr = save;
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
