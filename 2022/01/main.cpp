#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>

using ll = int64_t;
using values_t = std::vector<ll>;
using input_t = std::vector<values_t>;

input_t readInput() {
  input_t inputValuesAll;
  values_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      inputValuesAll.push_back(inputValues);
      inputValues.clear();
      continue;
    }
    std::istringstream iss(line);
    ll n;
    if (!(iss >> n)) {
      break;
    } // error

    inputValues.push_back(n);
  }
  inputValuesAll.push_back(inputValues);
  return inputValuesAll;
}

values_t getElfCalories(input_t const &aInputData) {
  values_t elfCalories;
  std::transform(std::cbegin(aInputData), std::cend(aInputData),
                 std::back_inserter(elfCalories), [](const auto &aCalories) {
                   return std::accumulate(
                       std::cbegin(aCalories), std::cend(aCalories), 0,
                       [](const auto &aCaloriesUntilNow, const auto &aCalorie) {
                         return aCaloriesUntilNow + aCalorie;
                       });
                 });
  return elfCalories;
}

ll solve1(input_t const &aInputData) {
  values_t elfCalories = getElfCalories(aInputData);
  return *std::max_element(std::cbegin(elfCalories), std::cend(elfCalories));
}

ll solve2(input_t const &aInputData) {
  values_t elfCalories = getElfCalories(aInputData);
  auto middle = std::next(std::begin(elfCalories), 3);
  std::partial_sort(std::begin(elfCalories), middle, std::end(elfCalories),
                    std::greater<int>());
  return std::accumulate(
      std::cbegin(elfCalories), std::next(std::cbegin(elfCalories), 3), 0,
      [](auto const &base, auto const &elem) { return base + elem; });
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}