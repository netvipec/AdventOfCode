#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;
using input_t = std::vector<std::string>;

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    inputValues.push_back(line);
  }
  return inputValues;
}

ll solve1(input_t const &aInputData) {
  return std::accumulate(
      std::cbegin(aInputData), std::cend(aInputData), 0,
      [](auto const &base, auto const &row) {
        std::set<int> firstCompartiment;
        std::set<int> secondCompartiment;

        auto toPriority = [](auto const &itemType) {
          return ('a' <= itemType && itemType <= 'z') ? itemType - 'a' + 1
                                                      : itemType - 'A' + 27;
        };

        std::transform(
            std::cbegin(row), std::next(std::cbegin(row), row.size() / 2),
            std::inserter(firstCompartiment, firstCompartiment.end()),
            toPriority);

        std::transform(
            std::next(std::cbegin(row), row.size() / 2), std::cend(row),
            std::inserter(secondCompartiment, secondCompartiment.end()),
            toPriority);

        std::vector<int> commonItemTypes;
        std::set_intersection(
            std::cbegin(firstCompartiment), std::cend(firstCompartiment),
            std::cbegin(secondCompartiment), std::cend(secondCompartiment),
            std::back_inserter(commonItemTypes));

        assert(commonItemTypes.size() == 1);
        return base + commonItemTypes.front();
      });
}

ll solve2(input_t const &aInputData) {
  auto toPriority = [](auto const &itemType) {
    return ('a' <= itemType && itemType <= 'z') ? itemType - 'a' + 1
                                                : itemType - 'A' + 27;
  };

  ll result = 0;
  std::vector<int> finalCommonItemTypes;
  std::set<int> partialCommonItemTypes;
  std::vector<std::set<int>> rucksack(3);
  for (size_t i = 0; i < aInputData.size(); i += 3) {
    for (size_t j = 0; j < 3; j++) {
      rucksack[j].clear();
      auto const &row = aInputData[i + j];
      std::transform(std::cbegin(row), std::cend(row),
                     std::inserter(rucksack[j], rucksack[j].end()), toPriority);
    }

    std::set_intersection(
        std::cbegin(rucksack[0]), std::cend(rucksack[0]),
        std::cbegin(rucksack[1]), std::cend(rucksack[1]),
        std::inserter(partialCommonItemTypes, partialCommonItemTypes.end()));
    std::set_intersection(std::cbegin(rucksack[2]), std::cend(rucksack[2]),
                          std::cbegin(partialCommonItemTypes),
                          std::cend(partialCommonItemTypes),
                          std::back_inserter(finalCommonItemTypes));

    assert(finalCommonItemTypes.size() == 1);
    result += finalCommonItemTypes.front();
    finalCommonItemTypes.clear();
    partialCommonItemTypes.clear();
  }
  return result;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}