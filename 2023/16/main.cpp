#include <algorithm>
#include <bits/stdc++.h>
#include <deque>
#include <iterator>
#include <numeric>
#include <optional>
#include <ratio>
#include <unordered_map>
#include <utility>

using ll = int64_t;
using input_t = std::vector<std::vector<char>>;

enum class Direction { Up, Right, Down, Left };

struct Position {
  int row{0};
  int col{0};

  bool operator<(Position const &other) const {
    return std::tie(row, col) < std::tie(other.row, other.col);
  }
};
struct Movement {
  Position pos{0, 0};
  Direction dir{Direction::Right};

  bool operator<(Movement const &other) const {
    return std::tie(pos, dir) < std::tie(other.pos, other.dir);
  }
};

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues.push_back(std::vector<char>{});
    std::transform(std::cbegin(line), std::cend(line),
                   std::back_inserter(inputValues.back()),
                   [](auto const &c) { return c; });
  }
  return inputValues;
}

std::optional<Position> move(Position pos, Direction dir, Position dimensions) {
  int row = pos.row;
  int col = pos.col;
  switch (dir) {
  case Direction::Up:
    row--;
    break;
  case Direction::Down:
    row++;
    break;
  case Direction::Left:
    col--;
    break;
  case Direction::Right:
    col++;
    break;
  }
  if (row < 0 || row >= dimensions.row || col < 0 || col >= dimensions.col) {
    return {};
  }
  return Position{row, col};
}

void print(std::set<Position> const &pos, Position dimensions) {
  for (int row = 0; row < dimensions.row; row++) {
    for (int col = 0; col < dimensions.col; col++) {
      std::cout << ((pos.find(Position{row, col}) != std::cend(pos)) ? "#"
                                                                     : ".");
    }
    std::cout << std::endl;
  }
}

void print(std::set<Movement> const &seen, Position dimensions) {
  std::set<Position> seen_pos;
  std::transform(std::cbegin(seen), std::cend(seen),
                 std::inserter(seen_pos, seen_pos.end()),
                 [](auto const &m) { return m.pos; });
  print(seen_pos, dimensions);
}

ll expand_from_pos(input_t const &aInputData, Movement start) {
  Position dimensions{static_cast<int>(aInputData.size()),
                      static_cast<int>(aInputData.front().size())};
  std::set<Movement> seen;

  std::deque<Movement> expand = {start};
  while (expand.size() > 0) {
    auto const m = expand.front();
    expand.pop_front();

    if (seen.find(m) != std::cend(seen)) {
      continue;
    }
    seen.insert(m);

    // print(seen, dimensions);
    // std::cout << std::endl;

    switch (aInputData[m.pos.row][m.pos.col]) {
    case '.': {
      auto np = move(m.pos, m.dir, dimensions);
      if (np) {
        expand.push_back(Movement{*np, m.dir});
      }
      break;
    }
    case '\\': {
      Direction nd = m.dir;
      if (m.dir == Direction::Up || m.dir == Direction::Down) {
        nd = static_cast<Direction>((static_cast<int>(nd) + 3) % 4);
      } else {
        nd = static_cast<Direction>((static_cast<int>(nd) + 1) % 4);
      }
      auto np = move(m.pos, nd, dimensions);
      if (np) {
        expand.push_back(Movement{*np, nd});
      }
      break;
    }
    case '/': {
      Direction nd = m.dir;
      if (m.dir == Direction::Up || m.dir == Direction::Down) {
        nd = static_cast<Direction>((static_cast<int>(nd) + 1) % 4);
      } else {
        nd = static_cast<Direction>((static_cast<int>(nd) + 3) % 4);
      }
      auto np = move(m.pos, nd, dimensions);
      if (np) {
        expand.push_back(Movement{*np, nd});
      }
      break;
    }
    case '-': {
      if (m.dir == Direction::Right || m.dir == Direction::Left) {
        auto np = move(m.pos, m.dir, dimensions);
        if (np) {
          expand.push_back(Movement{*np, m.dir});
        }
        break;
      }

      expand.push_back(Movement{m.pos, Direction::Left});
      expand.push_back(Movement{m.pos, Direction::Right});
      break;
    }
    case '|': {
      if (m.dir == Direction::Up || m.dir == Direction::Down) {
        auto np = move(m.pos, m.dir, dimensions);
        if (np) {
          expand.push_back(Movement{*np, m.dir});
        }
        break;
      }

      expand.push_back(Movement{m.pos, Direction::Up});
      expand.push_back(Movement{m.pos, Direction::Down});
      break;
    }
    }
  }

  std::set<Position> seen_pos;
  std::transform(std::cbegin(seen), std::cend(seen),
                 std::inserter(seen_pos, seen_pos.end()),
                 [](auto const &m) { return m.pos; });
  // print(seen_pos, dimensions);

  return seen_pos.size();
}

ll solve1(input_t const &aInputData) {
  return expand_from_pos(aInputData, Movement{});
}

ll solve2(input_t const &aInputData) {
  ll max = 0;
  for (int row = 0; row < static_cast<int>(aInputData.size()); row++) {
    max =
        std::max(max, expand_from_pos(aInputData, Movement{Position{row, 0},
                                                           Direction::Right}));
    max = std::max(
        max, expand_from_pos(
                 aInputData,
                 Movement{Position{row, static_cast<int>(
                                            aInputData.front().size() - 1)},
                          Direction::Left}));
  }
  for (int col = 0; col < static_cast<int>(aInputData.front().size()); col++) {
    max = std::max(max, expand_from_pos(aInputData, Movement{Position{0, col},
                                                             Direction::Down}));
    max = std::max(
        max,
        expand_from_pos(
            aInputData,
            Movement{Position{static_cast<int>(aInputData.size() - 1), col},
                     Direction::Up}));
  }
  return max;
}

int main() {
  auto const inputData = readInput();

  // std::for_each(std::cbegin(inputData), std::cend(inputData),
  //               [](auto const &elem) {
  //                 std::for_each(std::cbegin(elem), std::cend(elem),
  //                               [](auto const &e) { std::cout << e; });
  //                 std::cout << std::endl;
  //               });
  std::cout << std::endl;

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}