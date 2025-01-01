#include <algorithm>
#include <bits/stdc++.h>
#include <cassert>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;

struct Cards {
  std::vector<int> iWinners;
  std::vector<int> iHand;
};

using input_t = std::vector<Cards>;

static std::vector<std::string> split(std::string const &s,
                                      std::string const &delim = " ") {
  std::vector<std::string> elems;
  int start = 0;
  int end = s.find(delim);
  while (end != -1) {
    elems.push_back(s.substr(start, end - start));
    start = end + delim.size();
    end = s.find(delim, start);
  }
  elems.push_back(s.substr(start, end - start));
  return elems;
}

static std::vector<std::string> split(std::string const &s, char delim,
                                      bool removeEmpty = false) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    if (removeEmpty && item.empty())
      continue;
    elems.push_back(item);
  }
  return elems;
}

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    auto const cardParts = split(line, ": ");
    assert(cardParts.size() == 2);
    auto const numbersParts = split(cardParts[1], " | ");
    assert(numbersParts.size() == 2);

    auto const winnersParts = split(numbersParts[0], ' ', true);
    assert(winnersParts.size() > 0);
    auto const handsParts = split(numbersParts[1], ' ', true);
    assert(handsParts.size() > 0);

    Cards cards;
    std::transform(std::cbegin(winnersParts), std::cend(winnersParts),
                   std::back_inserter(cards.iWinners),
                   [](auto elem) { return std::stoi(elem); });
    std::transform(std::cbegin(handsParts), std::cend(handsParts),
                   std::back_inserter(cards.iHand),
                   [](auto elem) { return std::stoi(elem); });

    inputValues.push_back(cards);
  }
  return inputValues;
}

ll solve1(input_t const &aInputData) {
  return std::accumulate(
      std::cbegin(aInputData), std::cend(aInputData), 0,
      [](const auto &base, const Cards &elem) {
        auto const winnings = std::count_if(
            std::cbegin(elem.iHand), std::cend(elem.iHand),
            [&](const auto &cardNumber) {
              return std::find(std::cbegin(elem.iWinners),
                               std::cend(elem.iWinners),
                               cardNumber) != std::cend(elem.iWinners);
            });
        return base + (winnings > 0 ? (1 << (winnings - 1)) : 0);
      });
}

ll solve2(input_t const &aInputData) {
  std::map<int, int> cards;
  int index = 1;
  for (const auto &card : aInputData) {
    auto const winnings = std::count_if(
        std::cbegin(card.iHand), std::cend(card.iHand),
        [&](const auto &cardNumber) {
          return std::find(std::cbegin(card.iWinners), std::cend(card.iWinners),
                           cardNumber) != std::cend(card.iWinners);
        });
    for (int i = 0; i < winnings; i++) {
      cards[index + i + 1] += cards[index] + 1;
    }
    index++;
  }
  auto const res = std::accumulate(
      std::cbegin(cards), std::cend(cards), 0,
      [](const auto &base, const auto &elem) { return base + elem.second; });
  return res + aInputData.size();
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}