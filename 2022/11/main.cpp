#include <algorithm>
#include <bits/stdc++.h>
#include <cassert>
#include <functional>
#include <iterator>
#include <numeric>
#include <string>
#include <utility>

using ll = int64_t;

struct MonkeyInfo {
  ll id;
  std::vector<ll> items;
  std::vector<std::string> operation_rhs;
  ll divisibility_test;
  ll true_monkey_id;
  ll false_monkey_id;
};

using input_t = std::vector<MonkeyInfo>;

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

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](int ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }

    assert(line.find_first_of("Monkey") == 0);
    ll monkey_number = std::stoi(line.substr(7));

    std::getline(std::cin, line);
    trim(line);
    assert(line.find_first_of("Starting items: ") == 0);
    auto starting_elems_val = line.substr(16);
    auto starting_elems = split(starting_elems_val, ',');
    std::vector<ll> starting_elems_n;
    std::transform(std::cbegin(starting_elems), std::cend(starting_elems),
                   std::back_inserter(starting_elems_n), [](auto elem) {
                     trim(elem);
                     return std::stoi(elem);
                   });

    std::getline(std::cin, line);
    trim(line);
    assert(line.find_first_of("Operation: new = ") == 0);
    auto operation_elems_val = line.substr(17);
    auto operation_elems = split(operation_elems_val, ' ');

    std::getline(std::cin, line);
    trim(line);
    assert(line.find_first_of("Test: divisible by ") == 0);
    ll divisible_n = std::stoi(line.substr(19));

    std::getline(std::cin, line);
    trim(line);
    assert(line.find_first_of("If true: throw to monkey ") == 0);
    ll true_monkey_n = std::stoi(line.substr(25));

    std::getline(std::cin, line);
    trim(line);
    assert(line.find_first_of("If false: throw to monkey ") == 0);
    ll false_monkey_n = std::stoi(line.substr(26));

    inputValues.push_back(MonkeyInfo{monkey_number, starting_elems_n,
                                     operation_elems, divisible_n,
                                     true_monkey_n, false_monkey_n});
  }
  return inputValues;
}

ll calculate_operation(ll item, std::vector<std::string> const &operation) {
  assert(operation.size() == 3);
  ll lhs = (operation[0] == "old") ? item : std::stoi(operation[0]);
  ll rhs = (operation[2] == "old") ? item : std::stoi(operation[2]);
  if (operation[1] == "+") {
    return lhs + rhs;
  } else if (operation[1] == "*") {
    return lhs * rhs;
  } else {
    assert(false);
  }
  return -1;
}

ll solve1(input_t const &aInputData) {
  auto monkeyList = aInputData;
  std::vector<ll> processed_items(monkeyList.size());
  for (int round = 0; round < 20; round++) {
    std::for_each(
        std::begin(monkeyList), std::end(monkeyList), [&](auto &monkeyInfo) {
          processed_items[monkeyInfo.id] += monkeyInfo.items.size();
          std::for_each(
              std::cbegin(monkeyInfo.items), std::cend(monkeyInfo.items),
              [&](auto const &item) {
                auto const worry_level =
                    calculate_operation(item, monkeyInfo.operation_rhs) / 3;
                if (worry_level % monkeyInfo.divisibility_test == 0) {
                  monkeyList[monkeyInfo.true_monkey_id].items.push_back(
                      worry_level);
                } else {
                  monkeyList[monkeyInfo.false_monkey_id].items.push_back(
                      worry_level);
                }
              });
          monkeyInfo.items.clear();
        });
  }

  std::sort(std::begin(processed_items), std::end(processed_items),
            std::greater<>());

  assert(processed_items.size() > 2);
  return processed_items[0] * processed_items[1];
}

ll solve2(input_t const &aInputData) {
  auto const divisibility_mcm =
      std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 1ll,
                      [](auto const &base, auto const &monkey) {
                        return base * monkey.divisibility_test;
                      });

  auto monkeyList = aInputData;
  std::vector<ll> processed_items(monkeyList.size());
  for (int round = 0; round < 10000; round++) {
    std::for_each(
        std::begin(monkeyList), std::end(monkeyList), [&](auto &monkeyInfo) {
          processed_items[monkeyInfo.id] += monkeyInfo.items.size();
          std::for_each(
              std::cbegin(monkeyInfo.items), std::cend(monkeyInfo.items),
              [&](auto const &item) {
                auto const worry_level =
                    calculate_operation(item, monkeyInfo.operation_rhs) %
                    divisibility_mcm;
                if (worry_level % monkeyInfo.divisibility_test == 0) {
                  monkeyList[monkeyInfo.true_monkey_id].items.push_back(
                      worry_level);
                } else {
                  monkeyList[monkeyInfo.false_monkey_id].items.push_back(
                      worry_level);
                }
              });
          monkeyInfo.items.clear();
        });
  }

  std::sort(std::begin(processed_items), std::end(processed_items),
            std::greater<>());

  assert(processed_items.size() > 2);
  return processed_items[0] * processed_items[1];
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}