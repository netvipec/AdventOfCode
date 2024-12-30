#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;

enum class Operator { NOOP, ADD };

struct Instruction {
  Operator oper;
  std::vector<ll> param;
};

using input_t = std::vector<Instruction>;

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
    auto const elem = split(line, ' ');
    if (elem.size() == 1) {
      inputValues.push_back(Instruction{Operator::NOOP, {}});
      continue;
    } else {
      assert(elem.size() == 2);
      inputValues.push_back(Instruction{Operator::ADD, {std::stoi(elem[1])}});
    }
  }
  return inputValues;
}

ll solve1(input_t const &aInputData) {
  ll register_x = 1;
  ll cycles = 1;
  ll signal_strength = 0;
  bool finished = false;

  std::vector<ll> check_cycles{20, 60, 100, 140, 180, 220};
  auto check_cycles_it = check_cycles.cbegin();
  std::for_each(std::cbegin(aInputData), std::cend(aInputData),
                [&](auto const &instr) {
                  if (finished) {
                    return;
                  }

                  if (cycles == *check_cycles_it) {
                    signal_strength += register_x * *check_cycles_it;
                    ++check_cycles_it;
                    if (check_cycles_it == check_cycles.cend()) {
                      finished = true;
                    }
                  }

                  switch (instr.oper) {
                  case Operator::NOOP:
                    ++cycles;
                    break;
                  case Operator::ADD:
                    if (cycles + 1 == *check_cycles_it) {
                      signal_strength += register_x * *check_cycles_it;
                      ++check_cycles_it;
                      if (check_cycles_it == check_cycles.cend()) {
                        finished = true;
                      }
                    }

                    cycles += 2;
                    assert(instr.param.size() == 1);
                    register_x += instr.param[0];
                    break;
                  }
                });

  return signal_strength;
}

void update_screen(std::vector<std::string> &screen, ll cycles, ll register_x) {
  ll x = (cycles - 1) / 40;
  ll y = (cycles - 1) % 40;
  if (std::abs(y - register_x) < 2) {
    screen[x][y] = '*';
  } else {
    screen[x][y] = ' ';
  }
}

ll solve2(input_t const &aInputData) {
  ll register_x = 1;
  ll cycles = 1;

  std::vector<std::string> screen(6, std::string(40, ' '));

  std::for_each(std::cbegin(aInputData), std::cend(aInputData),
                [&](auto const &instr) {
                  update_screen(screen, cycles, register_x);

                  switch (instr.oper) {
                  case Operator::NOOP:
                    ++cycles;
                    break;
                  case Operator::ADD: {
                    update_screen(screen, cycles + 1, register_x);

                    cycles += 2;
                    assert(instr.param.size() == 1);
                    register_x += instr.param[0];

                    break;
                  }
                  }
                });

  std::for_each(std::cbegin(screen), std::cend(screen),
                [](auto const &line) { std::cout << line << std::endl; });
  return 0;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}