// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cctype>
#include <cassert>

using namespace std;

long expr(stringstream &ss);

long operand(stringstream &ss) {
  while (isspace(ss.peek()))
    ss.ignore(1);
  if (ss.peek() == '(') {
    ss.ignore(1);
    long v = expr(ss);
    while (isspace(ss.peek()))
      ss.ignore(1);
    assert(ss.peek() == ')');
    ss.ignore(1);
    return v;
  }
  long v;
  ss >> v;
  return v;
}

void reduce(vector<long> &rands, vector<char> &rations) {
  assert(rands.size() >= 2);
  assert(!rations.empty());
  long v2 = rands.back();
  rands.pop_back();
  long v1 = rands.back();
  rands.pop_back();
  char ration = rations.back();
  rations.pop_back();
  if (ration == '+')
    rands.push_back(v1 + v2);
  else
    rands.push_back(v1 * v2);
}

map<char, int> prec;

long expr(stringstream &ss) {
  vector<long> rands;
  rands.push_back(operand(ss));
  vector<char> rations;
  while (ss.peek(), !ss.eof()) {
    if (isspace(ss.peek())) {
      ss.ignore(1);
      continue;
    }
    if (ss.peek() == ')')
      break;
    char ration;
    ss >> ration;
    assert(ration == '+' || ration == '*');
    while (!rations.empty() && prec[ration] <= prec[rations.back()])
      reduce(rands, rations);
    rations.push_back(ration);
    rands.push_back(operand(ss));
  }
  while (!rations.empty())
    reduce(rands, rations);
  assert(rands.size() == 1);
  return rands.back();
}

void solve(int plus, int times) {
  prec['+'] = plus;
  prec['*'] = times;
  long ans = 0;
  string line;
  while (getline(cin, line)) {
    stringstream ss(line);
    ans += expr(ss);
  }
  cout << ans << '\n';
}


void part1() { solve(0, 0); }
void part2() { solve(1, 0); }

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
