// -*- C++ -*-
// Part 2 could possibly use optimization, but it's OKish (a couple of seconds)
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <array>
#include <list>
#include <set>
#include <functional>
#include <cassert>

using namespace std;

using decks = array<list<int>, 2>;

decks read() {
  decks deck;
  string line;
  for (int player = 0; player < 2; ++player) {
    getline(cin, line);
    while (getline(cin, line)) {
      if (line.empty())
        break;
      deck[player].push_back(stoi(line));
    }
  }
  return deck;
}

void score(decks &deck) {
  int winner = !deck[0].empty() ? 0 : 1;
  int ans = 0;
  int multiplier = 1;
  while (!deck[winner].empty()) {
    ans += multiplier * deck[winner].back();
    deck[winner].pop_back();
    ++multiplier;
  }
  cout << ans << '\n';
}

// Signature for detecting cycling, some random-ish hash
long sig(decks &deck) {
  long result = 0;
  for (int player = 0; player < 2; ++player) {
    for (auto card : deck[player])
      result = result * 997 + card;
    result *= 1009;
  }
  return result;
}

// Play the game, calling the given function to resolve the winner for
// each round.  Return the number of the winner (1 or 2)
int play(decks &deck, function<int(int, int, decks const &)> resolve) {
  set<long> prev;
  while (!deck[0].empty() && !deck[1].empty()) {
    long hash = sig(deck);
    if (prev.count(hash))
      // Cycling, player 1 wins
      return 1;
    prev.insert(hash);
    int card1 = deck[0].front();
    deck[0].pop_front();
    int card2 = deck[1].front();
    deck[1].pop_front();
    int round_winner = resolve(card1, card2, deck);
    if (round_winner == 1) {
      deck[0].push_back(card1);
      deck[0].push_back(card2);
    } else {
      deck[1].push_back(card2);
      deck[1].push_back(card1);
    }
  }
  return !deck[0].empty() ? 1 : 2;
}

// Recursive resolving function
int rec_resolve(int card1, int card2, decks const &deck) {
  assert(card1 != card2);
  if (int(deck[0].size()) < card1 || int(deck[1].size()) < card2)
    // Normal resolution
    return card1 > card2 ? 1 : 2;
  // Play a new subgame
  decks rec_deck;
  for (int player = 0; player < 2; ++player) {
    int num_cards = player == 0 ? card1 : card2;
    for (auto i = deck[player].begin(); num_cards-- > 0; ++i)
      rec_deck[player].push_back(*i);
  }
  return play(rec_deck, rec_resolve);
}

void part1() {
  auto deck = read();
  play(deck, [](int card1, int card2, decks const &) {
               assert(card1 != card2);
               return card1 > card2 ? 1 : 2;
             });
  score(deck);
}

void part2() {
  auto deck = read();
  play(deck, rec_resolve);
  score(deck);
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
