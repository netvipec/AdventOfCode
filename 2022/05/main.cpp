#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <utility>

using ll = int64_t;

struct MoveData {
  ll quantity;
  ll origin;
  ll destination;
};

struct CargoInfo {
  std::vector<std::vector<char>> stacks;
  std::vector<MoveData> movement;
};

using input_t = CargoInfo;

std::vector<std::vector<char>>
process_stacks(std::vector<std::string> const &stacks) {
  std::vector<std::vector<char>> order_stacks;

  ll stack_number = 0;
  std::unordered_map<ll, ll> stack_pos;
  auto const &numbers_row = stacks.back();
  for (size_t i = 0; i < numbers_row.size(); i++) {
    if ('0' <= numbers_row[i] && numbers_row[i] <= '9') {
      auto stack_n = static_cast<ll>(numbers_row[i] - '0');
      stack_pos.emplace(i, stack_n);
      stack_number = std::max(stack_number, stack_n);
    }
  }

  order_stacks.resize(stack_number);

  std::for_each(
      std::next(std::crbegin(stacks)), std::crend(stacks),
      [&](auto const &row) {
        std::for_each(
            std::cbegin(stack_pos), std::cend(stack_pos), [&](auto const &pos) {
              if (pos.first < static_cast<ll>(row.size()) &&
                  'A' <= row[pos.first] && row[pos.first] <= 'Z') {
                order_stacks[pos.second - 1].push_back(row[pos.first]);
              }
            });
      });

  return order_stacks;
}

input_t readInput() {
  input_t inputValues;
  std::vector<std::string> stacks;
  bool stacksRead = false;

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    if (line.empty() && !stacksRead) {
      inputValues.stacks = process_stacks(stacks);
      stacksRead = true;
      continue;
    }
    if (!stacksRead) {
      stacks.push_back(line);
      continue;
    }

    // move 1 from 2 to 1
    auto idx = line.find(' ', 5);
    auto idx2 = line.find(' ', idx + 6);

    auto n1 = std::stoi(line.substr(5, idx - 5));
    auto n2 = std::stoi(line.substr(idx + 6, idx2 - idx - 6));
    auto n3 = std::stoi(line.substr(idx2 + 4));

    inputValues.movement.push_back(MoveData{n1, n2 - 1, n3 - 1});
  }
  return inputValues;
}

void apply_movement1(std::vector<std::vector<char>> &stacks,
                     MoveData const &movement) {
  for (size_t i = 0; i < static_cast<size_t>(movement.quantity); i++) {
    stacks[movement.destination].push_back(
        stacks[movement.origin][stacks[movement.origin].size() - 1 - i]);
  }
  stacks[movement.origin].resize(stacks[movement.origin].size() -
                                 movement.quantity);
}

void apply_movement2(std::vector<std::vector<char>> &stacks,
                     MoveData const &movement) {
  for (size_t i = 0; i < static_cast<size_t>(movement.quantity); i++) {
    stacks[movement.destination].push_back(
        stacks[movement.origin]
              [stacks[movement.origin].size() - movement.quantity + i]);
  }
  stacks[movement.origin].resize(stacks[movement.origin].size() -
                                 movement.quantity);
}

std::string solve1(input_t const &aInputData) {
  auto stacks = aInputData.stacks;
  std::for_each(
      std::cbegin(aInputData.movement), std::cend(aInputData.movement),
      [&](auto const &movement) { apply_movement1(stacks, movement); });
  return std::accumulate(
      std::cbegin(stacks), std::cend(stacks), std::string{},
      [](auto const &base, auto const &stack) { return base + stack.back(); });
}

std::string solve2(input_t const &aInputData) {
  auto stacks = aInputData.stacks;
  std::for_each(
      std::cbegin(aInputData.movement), std::cend(aInputData.movement),
      [&](auto const &movement) { apply_movement2(stacks, movement); });
  return std::accumulate(
      std::cbegin(stacks), std::cend(stacks), std::string{},
      [](auto const &base, auto const &stack) { return base + stack.back(); });
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}