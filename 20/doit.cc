// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <optional>
#include <algorithm>
#include <cassert>

using namespace std;

struct tile {
  // Unique ID for the tile (0 for assembled full image)
  int id;
  // This includes borders for tiles, without for full image
  vector<string> image;
  // For tiles, all possible border configurations for various flips
  // and rotations
  set<string> possible_borders;

  // Construct a tile from stdin
  tile(int id_);
  // Assemble the full image (n * n tiles) from a list of
  // appropriately-oriented tiles
  tile(size_t n, vector<tile const *> const &by_columns);

  // Rotate 90 degrees left
  void rotate();
  // Flip top-to-bottom
  void flip();
  // Flip and rotate so that the top border matches the one given
  void align(string const &border);

  // Return various borders
  string top_border() const { return image.front(); }
  string bottom_border() const;
  string right_border() const;

  // Contents of the image of a tile with the borders removed
  vector<string> trim() const;

  // Find occurrences of a pattern.  Return nullopt if none, else the
  // number of set pixels that are not part of any occurrence.
  // Optionally show the image and the occurrences of the pattern.
  optional<size_t> occurrences(vector<string> const &pattern, bool show) const;
};

tile::tile(int id_) : id(id_) {
  string row;
  while (getline(cin, row)) {
    if (row.empty())
      break;
    image.push_back(row);
    assert(row.length() == image.front().length());
  }
  assert(!image.empty());
  assert(image.size() == image.front().length());
  for (int f = 0; f < 2; ++f) {
    flip();
    for (int r = 0; r < 4; ++r) {
      rotate();
      possible_borders.insert(top_border());
    }
  }
}

tile::tile(size_t n, vector<tile const *> const &by_columns) : id(0) {
  assert(n > 0 && by_columns.size() == n * n);
  size_t m = by_columns.front()->trim().size();
  image.resize(n * m);
  for (size_t i = 0; i < n * n; ++i) {
    auto trimmed = by_columns[i]->trim();
    assert(trimmed.size() == m);
    for (size_t j = 0; j < trimmed.size(); ++j)
      image[(i % n) * m + j] += trimmed[j];
  }
}

void tile::rotate() {
  auto rot = image;
  int n = image.size();
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      rot[i][j] = image[j][n - 1 - i];
  image = rot;
}

void tile::flip() {
  reverse(image.begin(), image.end());
}

void tile::align(string const &border) {
  for (int f = 0; f < 2; ++f) {
    flip();
    for (int r = 0; r < 4; ++r) {
      rotate();
      if (top_border() == border)
        return;
    }
  }
  // This should not be reached
  assert(possible_borders.find(border) != possible_borders.end());
}

string tile::bottom_border() const {
  string result = image.back();
  reverse(result.begin(), result.end());
  return result;
}

string tile::right_border() const {
  string result;
  for (auto const &row : image)
    result.push_back(row.back());
  return result;
}

vector<string> tile::trim() const {
  assert(image.size() >= 2);
  vector<string> result;
  for (size_t i = 1; i + 1 < image.size(); ++i)
    result.push_back(image[i].substr(1, image[i].length() - 2));
  return result;
}

optional<size_t> tile::occurrences(vector<string> const &pattern,
                                   bool show) const {
  set<pair<size_t, size_t>> in_pattern;
  size_t h = image.size();
  size_t w = image.front().size();
  size_t ph = pattern.size();
  size_t pw = pattern.front().size();
  auto occurs =
    [&](size_t i, size_t j) {
      // If the pattern occurs at upper left corner (i, j), add all
      // (offset) pattern pixels to in_pattern
      for (size_t i1 = 0; i1 < ph; ++i1)
        for (size_t j1 = 0; j1 < pw; ++j1)
          if (pattern[i1][j1] == '#' && image[i + i1][j + j1] != '#')
            return;
      for (size_t i1 = 0; i1 < ph; ++i1)
        for (size_t j1 = 0; j1 < pw; ++j1)
          if (pattern[i1][j1] == '#')
            in_pattern.emplace(i + i1, j + j1);
    };
  for (size_t i = 0; i + ph - 1 < h; ++i)
    for (size_t j = 0; j + pw - 1 < w; ++j)
      occurs(i, j);
  if (in_pattern.empty())
    // No occurrences
    return nullopt;
   size_t total_pixels = 0;
   for (size_t i = 0; i < h; ++i)
     total_pixels += count(image[i].begin(), image[i].end(), '#');
   if (show)
     for (size_t i = 0; i < h; ++i) {
       for (size_t j = 0; j < w; ++j)
         cout << (in_pattern.count({ i, j }) ? 'O' : image[i][j]);
       cout << '\n';
     }
   return total_pixels - in_pattern.size();
}

vector<tile> tiles;
map<string, set<int>> borders;
map<int, set<int>> pairs_with;

void read() {
  string line;
  while (getline(cin, line)) {
    assert(line.substr(0, 5) == "Tile ");
    tiles.emplace_back(stoi(line.substr(5)));
  }
  // Collect info on what tiles can match with what other ones
  for (auto const &t : tiles)
    for (auto const &border : t.possible_borders)
      borders[border].insert(t.id);
  for (auto const & [_, ids] : borders)
    for (auto id1 : ids)
      for (auto id2 : ids)
        if (id1 != id2) {
          pairs_with[id1].insert(id2);
          pairs_with[id2].insert(id1);
        }
}

tile *find_tile(int id) {
  tile *result = nullptr;
  for (auto &t : tiles)
    if (t.id == id)
      result = &t;
  assert(result);
  return result;
}

tile assemble() {
  // Find a corner
  optional<int> corner_id;
  for (auto const & [id, matches] : pairs_with)
    if (matches.size() == 2)
      corner_id = id;
  assert(corner_id);
  auto &corner = *find_tile(*corner_id);
  // Orient so top and left are unmatchable
  while (borders.find(corner.top_border())->second.size() != 1)
    corner.rotate();
  corner.rotate();
  if (borders.find(corner.top_border())->second.size() != 1)
    // Too far, unmatchable edges are like this: |_
    for (int _ = 0; _ < 3; ++_)
      corner.rotate();
  assert(borders.find(corner.top_border())->second.size() == 1);
  // How big is the picture?
  size_t n;
  for (n = 1; n * n < tiles.size(); ++n)
    ;
  assert(n * n == tiles.size());
  // Now it's just a matter of scanning top->bottom and left->right to
  // put things together.  At each point there should be only one tile
  // that can fit.
  vector<tile const *> by_columns{ &corner };
  set<int> used{ corner.id };
  while (by_columns.size() < tiles.size()) {
    string next_border;
    if (by_columns.size() % n == 0)
      // Match the right side of the tile just to the left
      next_border = by_columns[by_columns.size() - n]->right_border();
    else
      // Match the bottom side of the tile just above
      next_border = by_columns.back()->bottom_border();
    reverse(next_border.begin(), next_border.end());
    tile *next = nullptr;
    for (auto id : borders.find(next_border)->second)
      if (!used.count(id))
        next = find_tile(id);
    assert(next);
    next->align(next_border);
    if (by_columns.size() % n == 0)
      // This tile's left border should match the previous right
      // border.  Alignment was as-if the match was for the top, so
      // rotate one more time to put the matched side on the left
      next->rotate();
    by_columns.push_back(next);
    used.insert(next->id);
  }
  return tile(n, by_columns);
}

vector<string> monster{
  "                  # ",
  "#    ##    ##    ###",
  " #  #  #  #  #  #   "
};

void part1() {
  read();
  int num_corners = 0;
  long ans = 1;
  for (auto const & [id, matches] : pairs_with)
    // Corners can only match with two other tiles
    if (matches.size() == 2) {
      ++num_corners;
      ans *= id;
    }
  assert(num_corners == 4);
  cout << ans << '\n';
}

void part2() {
  read();
  auto full = assemble();
  for (int f = 0; f < 2; ++f) {
    full.flip();
    for (int r = 0; r < 4; ++r) {
      full.rotate();
      auto roughness = full.occurrences(monster, false);
      if (roughness) {
        cout << *roughness << '\n';
        return;
      }
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
