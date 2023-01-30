// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <algorithm>
#include <cassert>

using namespace std;

vector<map<string, string>> read() {
  vector<map<string, string>> result;
  map<string, string> passport;
  string line;
  while (getline(cin, line)) {
    if (line.empty()) {
      result.push_back(passport);
      passport.clear();
      continue;
    }
    stringstream ss(line);
    string field;
    while (ss >> field) {
      auto pos = field.find(':');
      assert(pos != string::npos);
      passport[field.substr(0, pos)] = field.substr(pos + 1);
    }
  }
  if (!passport.empty())
    result.push_back(passport);
  return result;
}

void count_valid(function<bool(map<string, string>)> valid) {
  auto passports = read();
  cout << count_if(passports.begin(), passports.end(), valid) << '\n';
}

bool digits(string const &s, unsigned n, string const &allowed) {
  if (s.length() != n)
    return false;
  return s.find_first_not_of(allowed) == string::npos;
}

bool year(string const &s, unsigned miny, unsigned maxy) {
  if (!digits(s, 4, "0123456789"))
    return false;
  unsigned y = stoi(s);
  return miny <= y && y <= maxy;
}

void part1() {
  vector<string> required{ "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
  count_valid([&](map<string, string> const &passport) {
                for (auto const &field: required)
                  if (!passport.count(field))
                    return false;
                return true;
              });
}

void part2() {
  set<string> valid_ecl{ "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
  count_valid([&](map<string, string> passport) {
                if (!year(passport["byr"], 1920, 2002)) return false;
                if (!year(passport["iyr"], 2010, 2020)) return false;
                if (!year(passport["eyr"], 2020, 2030)) return false;
                if (!digits(passport["pid"], 9, "0123456789")) return false;
                stringstream hgt(passport["hgt"]);
                unsigned v;
                string units;
                if (!(hgt >> v >> units)) return false;
                string _;
                if (hgt >> _) return false;
                if (units != "cm" && units != "in") return false;
                if (units == "cm" && !(150 <= v && v <= 193)) return false;
                if (units == "in" && !(59 <= v && v <= 76)) return false;
                auto hcl = passport["hcl"];
                if (hcl.length() != 7 || hcl[0] != '#' ||
                    !digits(hcl.substr(1), 6, "0123456789abcdef"))
                  return false;
                if (!valid_ecl.count(passport["ecl"])) return false;
                return true;
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
