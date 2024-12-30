#include <bits/stdc++.h>

using ll = int64_t;

struct Monkey {
  std::string name;
  ll number;
  std::string lhs;
  std::string op;
  std::string rhs;
};

using input_t = std::vector<Monkey>;

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
    auto const elems = split(line, ": ");
    assert(elems.size() == 2);
    auto const op_elems = split(elems[1], ' ');

    if (op_elems.size() == 1) {
      inputValues.push_back(Monkey{elems[0], std::stoi(elems[1])});
    } else {
      assert(op_elems.size() == 3);
      assert(op_elems[1].size() == 1);
      inputValues.push_back(
          Monkey{elems[0], -1, op_elems[0], op_elems[1], op_elems[2]});
    }
  }
  return inputValues;
}

ll calculate(Monkey const &monkey,
             std::map<std::string, ll> const &monkey_numbers) {
  if (monkey.number >= 0) {
    return monkey.number;
  }

  auto const lhs_it = monkey_numbers.find(monkey.lhs);
  auto const rhs_it = monkey_numbers.find(monkey.rhs);
  if (lhs_it != std::cend(monkey_numbers) &&
      rhs_it != std::cend(monkey_numbers)) {
    if (monkey.op == "+") {
      return lhs_it->second + rhs_it->second;
    } else if (monkey.op == "-") {
      return lhs_it->second - rhs_it->second;
    } else if (monkey.op == "*") {
      return lhs_it->second * rhs_it->second;
    } else if (monkey.op == "/") {
      return lhs_it->second / rhs_it->second;
    } else {
      assert(false);
    }
  }
  return -1;
}

ll solve1(input_t const &aInputData) {
  std::map<std::string, ll> monkey_numbers;
  constexpr char const *monkey_target = "root";

  bool updated = false;
  do {
    updated = false;
    std::for_each(std::cbegin(aInputData), std::cend(aInputData),
                  [&](auto const &monkey) {
                    if (monkey_numbers.find(monkey.name) !=
                        std::cend(monkey_numbers)) {
                      return;
                    }

                    ll number = calculate(monkey, monkey_numbers);

                    if (number >= 0) {
                      monkey_numbers[monkey.name] = number;
                      updated = true;
                    }
                  });

    if (monkey_numbers.find(monkey_target) != std::cend(monkey_numbers)) {
      break;
    }
  } while (updated);

  return monkey_numbers[monkey_target];
}

ll calculate_inverse(Monkey const &monkey,
                     std::map<std::string, ll> const &monkey_numbers) {
  assert(monkey.number < 0);

  auto const own_it = monkey_numbers.find(monkey.name);
  auto const lhs_it = monkey_numbers.find(monkey.lhs);
  auto const rhs_it = monkey_numbers.find(monkey.rhs);
  if (own_it == std::cend(monkey_numbers)) {
    return -1;
  }
  assert(lhs_it != std::cend(monkey_numbers) ||
         rhs_it != std::cend(monkey_numbers));

  auto &it = lhs_it != std::cend(monkey_numbers) ? lhs_it : rhs_it;
  if (monkey.op == "+") {
    return own_it->second - it->second;
  } else if (monkey.op == "-") {
    if (it == rhs_it) {
      return own_it->second + it->second;
    } else {
      return it->second - own_it->second;
    }
  } else if (monkey.op == "*") {
    return own_it->second / it->second;
  } else if (monkey.op == "/") {
    if (it == rhs_it) {
      return own_it->second * it->second;
    } else {
      return it->second / own_it->second;
    }
  } else {
    assert(false);
  }

  return -1;
}

ll backtrack(input_t const &input, std::map<std::string, ll> &monkey_numbers) {
  constexpr char const *monkey_target = "humn";

  bool updated = false;
  do {
    updated = false;
    std::for_each(
        std::cbegin(input), std::cend(input), [&](auto const &monkey) {
          if (monkey.number >= 0) {
            return;
          }
          if (monkey_numbers.find(monkey.lhs) != std::cend(monkey_numbers) &&
              monkey_numbers.find(monkey.rhs) != std::cend(monkey_numbers)) {
            return;
          }
          if (monkey_numbers.find(monkey.lhs) == std::cend(monkey_numbers) &&
              monkey_numbers.find(monkey.rhs) == std::cend(monkey_numbers)) {
            return;
          }

          ll number = calculate_inverse(monkey, monkey_numbers);

          if (number >= 0) {
            auto const lhs_it = monkey_numbers.find(monkey.lhs);
            auto &other_name =
                lhs_it != std::cend(monkey_numbers) ? monkey.rhs : monkey.lhs;

            monkey_numbers[other_name] = number;
            updated = true;
          }
        });

    if (monkey_numbers.find(monkey_target) != std::cend(monkey_numbers)) {
      break;
    }
  } while (updated);

  return monkey_numbers[monkey_target];
}

ll solve2(input_t const &aInputData) {
  std::map<std::string, ll> monkey_numbers;
  constexpr char const *monkey_target = "root";
  constexpr char const *human_target = "humn";

  bool updated = false;
  do {
    updated = false;
    std::for_each(std::cbegin(aInputData), std::cend(aInputData),
                  [&](auto const &monkey) {
                    if (monkey.name == human_target) {
                      return;
                    }
                    if (monkey_numbers.find(monkey.name) !=
                        std::cend(monkey_numbers)) {
                      return;
                    }

                    ll number = calculate(monkey, monkey_numbers);

                    if (number >= 0) {
                      monkey_numbers[monkey.name] = number;
                      updated = true;
                    }
                  });

    if (monkey_numbers.find(monkey_target) != std::cend(monkey_numbers)) {
      break;
    }
  } while (updated);

  auto const root_monkey_it = std::find_if(
      std::cbegin(aInputData), std::cend(aInputData),
      [&](auto const &monkey) { return monkey.name == monkey_target; });
  assert(root_monkey_it != std::cend(aInputData));
  assert(root_monkey_it->number < 0);
  auto const root_lhs = monkey_numbers.find(root_monkey_it->lhs);
  auto const root_rhs = monkey_numbers.find(root_monkey_it->rhs);

  assert(root_lhs != std::cend(monkey_numbers) ||
         root_rhs != std::cend(monkey_numbers));

  if (root_lhs != std::cend(monkey_numbers)) {
    monkey_numbers[root_monkey_it->rhs] = root_lhs->second;
    return backtrack(aInputData, monkey_numbers);
  } else {
    monkey_numbers[root_monkey_it->lhs] = root_rhs->second;
    return backtrack(aInputData, monkey_numbers);
  }
  return -1;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}