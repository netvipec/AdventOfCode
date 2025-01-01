#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;
using input_t = std::vector<std::pair<std::string, ll>>;

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

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    auto const handParts = split(line, " ");
    assert(handParts.size() == 2);

    inputValues.emplace_back(handParts[0], std::stoll(handParts[1]));
  }
  return inputValues;
}

ll getCardTypeP1(const std::string &cards) {
  std::map<char, int> repeatedCards;
  std::vector<int> repeatedCardsCount(6);
  std::for_each(std::cbegin(cards), std::cend(cards),
                [&](const auto &card) { repeatedCards[card]++; });
  std::for_each(std::cbegin(repeatedCards), std::cend(repeatedCards),
                [&](const auto &repeatedCard) {
                  repeatedCardsCount[repeatedCard.second]++;
                });

  if (repeatedCards.size() == 5) {
    return 1;
  } else if (repeatedCards.size() == 4) {
    return 2;
  } else if (repeatedCards.size() == 3 && repeatedCardsCount[2] == 2) {
    return 3;
  } else if (repeatedCards.size() == 3 && repeatedCardsCount[3] == 1) {
    return 4;
  } else if (repeatedCards.size() == 2 && repeatedCardsCount[2] == 1 &&
             repeatedCardsCount[3] == 1) {
    return 5;
  } else if (repeatedCards.size() == 2 && repeatedCardsCount[4] == 1) {
    return 6;
  } else if (repeatedCards.size() == 1) {
    return 7;
  }
  assert(false);
  return -1;
}

ll getCardTypeP2(const std::string &cards) {
  std::map<char, int> repeatedCards;
  std::vector<int> repeatedCardsCount(6);
  ll joker = 0;
  std::for_each(std::cbegin(cards), std::cend(cards), [&](const auto &card) {
    if (card != 'J') {
      repeatedCards[card]++;
    } else {
      joker++;
    }
  });
  std::for_each(std::cbegin(repeatedCards), std::cend(repeatedCards),
                [&](const auto &repeatedCard) {
                  repeatedCardsCount[repeatedCard.second]++;
                });

  auto mostRepeatedIt = std::find_if(std::crbegin(repeatedCardsCount),
                                     std::crend(repeatedCardsCount),
                                     [](auto const &elem) { return elem > 0; });
  ll mostRepeated = 0;
  if (mostRepeatedIt != std::crend(repeatedCardsCount)) {
    mostRepeated = std::distance(std::cbegin(repeatedCardsCount),
                                 (mostRepeatedIt + 1).base());
  }
  if (mostRepeated + joker == 5) {
    return 7;
  } else if (mostRepeated + joker == 4) {
    return 6;
  } else if (mostRepeated + joker == 3 &&
             (repeatedCardsCount[2] == 2 ||
              (repeatedCardsCount[2] == 1 && repeatedCardsCount[3] == 1))) {
    return 5;
  } else if (mostRepeated + joker == 3) {
    return 4;
  } else if (mostRepeated + joker == 2 && repeatedCardsCount[2] == 2) {
    return 3;
  } else if (mostRepeated + joker == 2) {
    return 2;
  } else if (mostRepeated + joker == 1) {
    return 1;
  }
  assert(false);
  return -1;
}

const std::vector<char> kCardOrderP1{'2', '3', '4', '5', '6', '7', '8',
                                     '9', 'T', 'J', 'Q', 'K', 'A'};
const std::vector<char> kCardOrderP2{'J', '2', '3', '4', '5', '6', '7',
                                     '8', '9', 'T', 'Q', 'K', 'A'};

template <typename Functor>
bool compareCards(const std::string &lhs, const std::string &rhs,
                  const std::vector<char> &cardOrder, Functor functor) {
  auto leftType = functor(lhs);
  auto rightType = functor(rhs);
  if (leftType < rightType) {
    return true;
  }
  if (rightType < leftType) {
    return false;
  }

  for (size_t i = 0; i < lhs.size(); i++) {
    auto lIndex = std::distance(
        std::cbegin(cardOrder),
        std::find(std::cbegin(cardOrder), std::cend(cardOrder), lhs[i]));
    auto rIndex = std::distance(
        std::cbegin(cardOrder),
        std::find(std::cbegin(cardOrder), std::cend(cardOrder), rhs[i]));
    if (lIndex < rIndex) {
      return true;
    }
    if (rIndex < lIndex) {
      return false;
    }
  }
  return false;
}

ll solve1(input_t const &aInputData) {
  auto input = aInputData;
  std::sort(
      std::begin(input), std::end(input), [](const auto &lhs, const auto &rhs) {
        return compareCards(lhs.first, rhs.first, kCardOrderP1, getCardTypeP1);
      });

  ll acc = 0;
  for (size_t i = 0; i < input.size(); i++) {
    acc += (i + 1) * input[i].second;
  }
  return acc;
}

ll solve2(input_t const &aInputData) {
  auto input = aInputData;
  std::sort(
      std::begin(input), std::end(input), [](const auto &lhs, const auto &rhs) {
        return compareCards(lhs.first, rhs.first, kCardOrderP2, getCardTypeP2);
      });

  ll acc = 0;
  for (size_t i = 0; i < input.size(); i++) {
    acc += (i + 1) * input[i].second;
  }
  return acc;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}