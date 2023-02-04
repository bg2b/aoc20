// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <limits>
#include <cctype>
#include <cassert>

using namespace std;

// An individual character; or a vector of alternatives, each of which
// is a vector of sequential rules
using rule = variant<char, vector<vector<int>>>;

map<int, rule> rules;

// Lower and upper bounds on the lengths of possible matches for each
// rule
map<int, pair<unsigned, unsigned>> bounds;

// Get the length bounds for rule ri
pair<unsigned, unsigned> get_bounds(int ri) {
  auto p = bounds.find(ri);
  if (p != bounds.end())
    // Already computed
    return p->second;
  auto const &r = rules.find(ri)->second;
  if (r.index() == 0) {
    // Single character
    auto result = make_pair(1, 1);
    bounds.emplace(ri, result);
    return result;
  }
  optional<unsigned> min_len;
  optional<unsigned> max_len;
  auto const &alt = get<1>(r);
  for (auto const &seq : alt) {
    unsigned seq_min_len = 0;
    unsigned seq_max_len = 0;
    bool recursive = false;
    for (auto ri1 : seq) {
      if (ri == ri1) {
        // Note if this rule references itself
        recursive = true;
        continue;
      }
      auto [min1, max1] = get_bounds(ri1);
      seq_min_len += min1;
      seq_max_len += max1;
    }
    if (recursive)
      // Recursive rules can generate arbitrarily large results
      seq_max_len = numeric_limits<unsigned>::max();
    min_len = min(min_len.value_or(seq_min_len), seq_min_len);
    max_len = max(max_len.value_or(seq_max_len), seq_max_len);
  }
  auto result = make_pair(*min_len, *max_len);
  bounds.insert_or_assign(ri, result);
  return result;
}

void read_rules(bool tweak811) {
  string line;
  while (getline(cin, line)) {
    if (line.empty())
      return;
    if (tweak811) {
      if (line == "8: 42")
        line = "8: 42 | 42 8";
      else if (line == "11: 42 31")
        line = "11: 42 31 | 42 11 31";
    }
    stringstream ss(line);
    int rule_num;
    char colon;
    ss >> rule_num >> colon;
    auto peek = [&] {
                  while (isspace(ss.peek()))
                    ss.ignore(1);
                  return ss.peek();
                };
    if (peek() == '"') {
      string ch;
      ss >> ch;
      assert(ch.length() == 3);
      rules.emplace(rule_num, rule(ch[1]));
      continue;
    }
    vector<vector<int>> alt;
    while (true) {
      vector<int> seq;
      while (isdigit(peek())) {
        int rnum;
        ss >> rnum;
        seq.push_back(rnum);
      }
      alt.push_back(seq);
      if (peek() == '|') {
        ss.ignore(1);
        continue;
      }
      break;
    }
    rules.emplace(rule_num, rule(alt));
  }
}

bool match(int ri, string const &s, map<pair<int, string>, bool> &cache);

// Does a sequence of rules match something?
bool match_seq(vector<int> const &seq, string const &s,
               map<pair<int, string>, bool> &cache) {
  assert(!seq.empty());
  if (seq.size() == 1)
    // Sequence with a single rule
    return match(seq[0], s, cache);
  // What range of characters can the first rule in the sequence
  // match?
  auto [min0, max0] = get_bounds(seq[0]);
  if (max0 > s.length())
    max0 = s.length();
  for (auto split = min0; split <= max0; ++split) {
    if (!match(seq[0], s.substr(0, split), cache))
      continue;
    // The first part can match; try to match whatever's left
    vector<int> rest(seq.begin() + 1, seq.end());
    if (match_seq(rest, s.substr(split), cache))
      return true;
  }
  return false;
}

// Does a rule match something?
bool match(int ri, string const &s, map<pair<int, string>, bool> &cache) {
  auto [min_len, max_len] = get_bounds(ri);
  if (s.length() < min_len || s.length() > max_len)
    // Impossible based on length alone
    return false;
  assert(rules.find(ri) != rules.end());
  auto const &r = rules.find(ri)->second;
  if (r.index() == 0) {
    // Single character
    assert(s.length() == 1);
    return s[0] == get<0>(r);
  }
  auto key = make_pair(ri, s);
  auto p = cache.find(key);
  if (p != cache.end())
    // Already checked
    return p->second;
  auto const &alt = get<1>(r);
  assert(!alt.empty());
  bool result = false;
  // Match if any alternative matches
  for (auto const &seq : alt)
    if (match_seq(seq, s, cache)) {
      result = true;
      break;
    }
  cache.emplace(key, result);
  return result;
}

void solve(bool tweak811) {
  read_rules(tweak811);
  string message;
  map<pair<int, string>, bool> cache;
  int ans = 0;
  while (cin >> message)
    if (match(0, message, cache))
      ++ans;
  cout << ans << '\n';
}

void part1() { solve(false); }
void part2() { solve(true); }

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
