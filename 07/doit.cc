// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

using contents = map<string, vector<pair<int, string>>>;

contents read() {
  contents result;
  string line;
  while (getline(cin, line)) {
    stringstream ss(line);
    string _;
    string adj, col;
    ss >> adj >> col >> _ >> _;
    string color = adj + ' ' + col;
    auto p = result.emplace(color, vector<pair<int, string>>{}).first;
    auto &contents = p->second;
    if (line.find("no other") != string::npos)
      continue;
    unsigned num;
    while (ss >> num >> adj >> col >> _)
      contents.emplace_back(num, adj + ' ' + col);
  }
  return result;
}

bool holds_ours(string const &color, contents const &bags,
                map<string, bool> &cache) {
  auto p = cache.find(color);
  if (p != cache.end())
    return p->second;
  p = cache.emplace(color, false).first;
  for (auto const & [_, inside] : bags.find(color)->second)
    if (inside == "shiny gold" || holds_ours(inside, bags, cache))
      p->second = true;
  return p->second;
}

int bags_held(string const &color, contents const &bags,
              map<string, int> &cache) {
  auto p = cache.find(color);
  if (p != cache.end())
    return p->second;
  p = cache.emplace(color, 0).first;
  auto holds = bags.find(color)->second;
  for (auto const & [num, inside] : bags.find(color)->second)
    p->second += num * (bags_held(inside, bags, cache) + 1);
  return p->second;
}

void part1() {
  contents bags = read();
  int ans = 0;
  map<string, bool> cache;
  for (auto const & [color, _] : bags)
    if (holds_ours(color, bags, cache))
      ++ans;
  cout << ans << '\n';
}

void part2() {
  contents bags = read();
  map<string, int> cache;
  cout << bags_held("shiny gold", bags, cache) << '\n';
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
