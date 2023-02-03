// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <cassert>

using namespace std;

struct field {
  string name;
  vector<pair<int, int>> ranges;

  field(string const &s);

  bool compatible(int value) const;
};

field::field(string const &s) {
  auto pos = s.find(':');
  assert(pos != string::npos);
  name=s.substr(0, pos);
  stringstream ss(s.substr(pos));
  string _;
  unsigned low, high;
  char dash;
  while (ss >> _ >> low >> dash >> high) {
    assert(dash == '-');
    ranges.emplace_back(low, high);
  }
}

bool field::compatible(int value) const {
  for (auto [low, high] : ranges)
    if (low <= value && value <= high)
      return true;
  return false;
}

vector<field> fields;

struct ticket {
  vector<int> values;

  ticket(string s);

  // Return sum of values that are out-of-range of all fields.  Needs
  // an optional<int> since it's necessary to distinguish between a
  // sum of 0 (e.g., a ticket value of 0 that isn't valid) and "the
  // ticket could be valid"
  optional<int> scanning_errors() const;
};

ticket::ticket(string s) {
  s.push_back(',');
  stringstream ss(s);
  int value;
  char comma;
  while (ss >> value >> comma) {
    assert(comma == ',');
    values.push_back(value);
  }
  assert(values.size() == fields.size());
}

optional<int> ticket::scanning_errors() const {
  optional<int> errors;
  for (auto value : values) {
    bool valid_for_any = false;
    for (auto const &field : fields)
      valid_for_any = valid_for_any || field.compatible(value);
    if (!valid_for_any)
      errors = errors.value_or(0) + value;
  }
  return errors;
}

pair<ticket, vector<ticket>> read() {
  string line;
  while (getline(cin, line) && !line.empty())
    fields.emplace_back(line);
  getline(cin, line);
  assert(line == "your ticket:");
  getline(cin, line);
  ticket yours(line);
  getline(cin, line);
  getline(cin, line);
  assert(line == "nearby tickets:");
  vector<ticket> nearby;
  while (getline(cin, line))
    nearby.emplace_back(line);
  return { yours, nearby };
}

void part1() {
  auto [yours, nearby] = read();
  int ans = 0;
  for (auto const &t : nearby)
    ans += t.scanning_errors().value_or(0);
  cout << ans << '\n';
}

void part2() {
  auto [yours, nearby] = read();
  // Toss invalid tickets
  for (size_t i = 0; i < nearby.size(); )
    if (nearby[i].scanning_errors())
      nearby.erase(nearby.begin() + i);
    else
      ++i;
  auto tickets = move(nearby);
  tickets.push_back(yours);
  // Compatibility matrix, [index on ticket][index in fields], paired
  // with ticket index to keep track of where things came from
  int n = fields.size();
  vector<pair<vector<char>, int>> match;
  for (int i = 0; i < n; ++i)
    match.emplace_back(vector<char>(n, 1), i);
  for (int ti = 0; ti < n; ++ti)
    for (int fi = 0; fi < n; ++fi) {
      auto &compat = match[ti].first[fi];
      for (auto const &t : tickets)
        compat = compat && fields[fi].compatible(t.values[ti]);
    }
  // This is kind of a hack, but the input is apparently designed so
  // that a single simple inference is possible at every stage.
  // Sorting puts the thing that's compatible with a single field
  // first, then whatever's compatible with that field and one other
  // next, etc.
  sort(match.begin(), match.end());
  // Ticket-to-field mapping
  vector<int> t2f(n, -1);
  for (int i = 0; i < n; ++i) {
    int ti = match[i].second;
    for (int fi = 0; fi < n; ++fi)
      if (match[i].first[fi] && find(t2f.begin(), t2f.end(), fi) == t2f.end())
        // ticket index ti is compatible with field index fi, and fi
        // has not been used before
        t2f[ti] = fi;
  }
  // Product of departure fields
  long ans = 1;
  for (int i = 0; i < n; ++i) {
    auto const &field = fields[t2f[i]];
    if (field.name.find("departure") == 0)
      ans *= yours.values[i];
  }
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
