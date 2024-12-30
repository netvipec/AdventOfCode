#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;

struct SectionAssignment {
  ll startFirst;
  ll endFirst;
  ll startSecond;
  ll endSecond;
};

using input_t = std::vector<SectionAssignment>;

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
    auto const elems = split(line, ',');
    assert(elems.size() == 2);
    auto const firstElems = split(elems.front(), '-');
    assert(firstElems.size() == 2);
    auto const secondElems = split(elems.back(), '-');
    assert(secondElems.size() == 2);

    inputValues.push_back(SectionAssignment{
        std::stoi(firstElems.front()), std::stoi(firstElems.back()),
        std::stoi(secondElems.front()), std::stoi(secondElems.back())});
  }
  return inputValues;
}

ll solve1(input_t const &aInputData) {
  return std::count_if(
      std::cbegin(aInputData), std::cend(aInputData),
      [](auto const &assignment) {
        return (assignment.startFirst <= assignment.startSecond &&
                assignment.endSecond <= assignment.endFirst) ||
               (assignment.startSecond <= assignment.startFirst &&
                assignment.endFirst <= assignment.endSecond);
      });
}

ll solve2(input_t const &aInputData) {
  return std::count_if(
      std::cbegin(aInputData), std::cend(aInputData),
      [](auto const &assignment) {
        return (assignment.startFirst <= assignment.startSecond &&
                assignment.startSecond <= assignment.endFirst) ||
               (assignment.startSecond <= assignment.startFirst &&
                assignment.startFirst <= assignment.endSecond);
      });
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}