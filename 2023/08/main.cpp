#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;
struct data_t {
  std::string moves;
  std::map<std::string, std::pair<std::string, std::string>> binary_tree;
};
using input_t = data_t;

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

  std::getline(std::cin, inputValues.moves);

  std::string line;
  std::getline(std::cin, line);
  while (std::getline(std::cin, line)) {
    auto const parts = split(line, " = ");
    assert(parts.size() == 2);
    auto const right_value = parts[1].substr(1, parts[1].size() - 2);
    auto const right = split(right_value, ", ");
    assert(right.size() == 2);

    inputValues.binary_tree.emplace(parts[0],
                                    std::make_pair(right[0], right[1]));
  }
  return inputValues;
}

template <typename Functor>
ll get_steps(std::string const &node, Functor is_final_node,
             input_t const &aInputData) {
  ll steps = 0;
  auto actual_node = node;
  for (;;) {
    const auto m = aInputData.moves[steps % aInputData.moves.size()];
    steps++;
    assert(m == 'L' || m == 'R');

    const auto &node_it = aInputData.binary_tree.find(actual_node);
    assert(node_it != std::cend(aInputData.binary_tree));
    actual_node = m == 'L' ? node_it->second.first : node_it->second.second;
    if (is_final_node(actual_node)) {
      return steps;
    }
  }
}

ll solve1(input_t const &aInputData) {
  return get_steps(
      "AAA", [](auto const &node) { return node == "ZZZ"; }, aInputData);
}

ll solve2(input_t const &aInputData) {
  std::vector<ll> steps_to_final;
  for (auto const &node : aInputData.binary_tree) {
    if (node.first[node.first.size() - 1] == 'A') {
      steps_to_final.push_back(get_steps(
          node.first,
          [](auto const &actual_node) {
            return actual_node[actual_node.size() - 1] == 'Z';
          },
          aInputData));
    }
  }
  return std::accumulate(
      std::cbegin(steps_to_final), std::cend(steps_to_final), 1ll,
      [](auto const &base, auto const &elem) { return std::lcm(base, elem); });
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}