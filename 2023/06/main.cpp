#include <algorithm>
#include <bits/stdc++.h>
#include <cassert>
#include <iterator>
#include <numeric>
#include <ratio>
#include <utility>

using ll = int64_t;
struct data {
  std::vector<ll> time;
  std::vector<ll> distance;
};
using input_t = data;

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
  std::getline(std::cin, line);
  auto const time = split(line, ": ");
  assert(time.size() == 2);
  std::getline(std::cin, line);
  auto const distance = split(line, ": ");
  assert(distance.size() == 2);

  auto const timeElem = split(time[1], ' ', true);
  assert(timeElem.size() > 0);
  auto const distanceElem = split(distance[1], ' ', true);
  assert(distanceElem.size() > 0);

  std::transform(std::cbegin(timeElem), std::cend(timeElem),
                 std::back_inserter(inputValues.time),
                 [](auto const &elem) { return std::stoll(elem); });
  std::transform(std::cbegin(distanceElem), std::cend(distanceElem),
                 std::back_inserter(inputValues.distance),
                 [](auto const &elem) { return std::stoll(elem); });

  assert(inputValues.time.size() == inputValues.distance.size());
  return inputValues;
}

std::pair<ll, ll> solve_quadratic(ll time, ll distance) {
  auto const b1 = static_cast<ll>(
      std::ceil((time - std::sqrt(time * time - 4 * distance)) / 2));
  auto const b2 =
      static_cast<ll>((time + std::sqrt(time * time - 4 * distance)) / 2);
  return {b1, b2 + 1};
}

ll solve1(input_t const &aInputData) {
  ll mult = 1;
  for (size_t i = 0; i < aInputData.time.size(); i++) {
    auto range = solve_quadratic(aInputData.time[i], aInputData.distance[i]);
    mult *= (range.second - range.first);
  }
  return mult;
}

template <class T> int numDigits(T number) {
  int digits = 1;
  if (number < 0)
    digits = 1; // remove this line if '-' counts as a digit
  while (number) {
    number /= 10;
    digits *= 10;
  }
  return digits;
}

ll solve2(input_t const &aInputData) {
  const auto time =
      std::accumulate(std::cbegin(aInputData.time), std::cend(aInputData.time),
                      0ll, [](auto const &base, auto const &elem) {
                        const auto digits = numDigits(elem);
                        return base * digits + elem;
                      });
  const auto distance = std::accumulate(std::cbegin(aInputData.distance),
                                        std::cend(aInputData.distance), 0ll,
                                        [](auto const &base, auto const &elem) {
                                          const auto digits = numDigits(elem);
                                          return base * digits + elem;
                                        });
  auto const range = solve_quadratic(time, distance);
  return range.second - range.first;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}