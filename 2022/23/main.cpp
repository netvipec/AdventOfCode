#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <utility>

using ll = int64_t;

struct Position {
  ll x;
  ll y;

  bool operator<(Position const &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
  bool operator==(Position const &other) const {
    return std::tie(x, y) == std::tie(other.x, other.y);
  }
};

using input_t = std::set<Position>;

input_t readInput() {
  input_t inputValues;

  ll x = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    for (ll y = 0; y < static_cast<ll>(line.size()); y++) {
      if (line[y] == '#') {
        inputValues.emplace(Position{x, y});
      }
    }

    x++;
  }
  return inputValues;
}

std::vector<Position> get_neiborgs() {
  std::vector<Position> p;
  for (ll x = -1; x <= 1; x++) {
    for (ll y = -1; y <= 1; y++) {
      if (x == 0 && y == 0) {
        continue;
      }

      p.push_back(Position{x, y});
    }
  }
  return p;
}

bool elve_present(input_t const &elve_positions, Position const &cell) {
  return elve_positions.find(cell) != std::cend(elve_positions);
}

struct Move {
  Position pre;
  Position post;
};

struct CheckData {
  bool valid;
  Position pos;
};

const std::vector<std::function<CheckData(input_t const &, Position const &)>>
    checks{[](input_t const &elve_positions, Position const &elve) {
             const Position north{elve.x - 1, elve.y};
             const Position north_west{elve.x - 1, elve.y - 1};
             const Position north_east{elve.x - 1, elve.y + 1};

             return CheckData{!elve_present(elve_positions, north) &&
                                  !elve_present(elve_positions, north_east) &&
                                  !elve_present(elve_positions, north_west),
                              north};
           },
           [](input_t const &elve_positions, Position const &elve) {
             const Position south{elve.x + 1, elve.y};
             const Position south_west{elve.x + 1, elve.y - 1};
             const Position south_east{elve.x + 1, elve.y + 1};

             return CheckData{!elve_present(elve_positions, south) &&
                                  !elve_present(elve_positions, south_east) &&
                                  !elve_present(elve_positions, south_west),
                              south};
           },
           [](input_t const &elve_positions, Position const &elve) {
             const Position west{elve.x, elve.y - 1};
             const Position north_west{elve.x - 1, elve.y - 1};
             const Position south_west{elve.x + 1, elve.y - 1};

             return CheckData{!elve_present(elve_positions, west) &&
                                  !elve_present(elve_positions, north_west) &&
                                  !elve_present(elve_positions, south_west),
                              west};
           },
           [](input_t const &elve_positions, Position const &elve) {
             const Position east{elve.x, elve.y + 1};
             const Position north_east{elve.x - 1, elve.y + 1};
             const Position south_east{elve.x + 1, elve.y + 1};

             return CheckData{!elve_present(elve_positions, east) &&
                                  !elve_present(elve_positions, north_east) &&
                                  !elve_present(elve_positions, south_east),
                              east};
           }};

void print_map(ll min_x, ll max_x, ll min_y, ll max_y,
               input_t const &elve_pos) {
  std::for_each(std::cbegin(elve_pos), std::cend(elve_pos),
                [](auto const &elve) {
                  std::cout << "elve: " << elve.x << "," << elve.y << std::endl;
                });
  for (ll x = min_x; x <= max_x; x++) {
    for (ll y = min_y; y <= max_y; y++) {
      auto it = elve_pos.find(Position{x, y});
      if (it != std::cend(elve_pos)) {
        std::cout << "#";
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool simulate_round(input_t &elve_pos, std::vector<Position> const &neigborgs,
                    ll round) {
  bool elve_move = false;
  std::vector<Move> proposed_move;
  std::for_each(
      std::cbegin(elve_pos), std::cend(elve_pos), [&](auto const &elve) {
        auto const neiborgs_count =
            std::count_if(std::cbegin(neigborgs), std::cend(neigborgs),
                          [&](auto const &move) {
                            auto const present_it = elve_pos.find(
                                Position{elve.x + move.x, elve.y + move.y});
                            return present_it != std::cend(elve_pos);
                          });

        if (neiborgs_count == 0) {
          return;
        }

        for (int i = 0; i < 4; i++) {
          auto const &check = checks[(i + round) % checks.size()];
          auto const res = check(elve_pos, elve);
          if (res.valid) {
            proposed_move.push_back(Move{elve, res.pos});
            break;
          }
        }
      });

  for (auto move_it = std::cbegin(proposed_move);
       move_it != std::cend(proposed_move); move_it++) {
    bool invalid = false;
    for (auto other_move_it = std::cbegin(proposed_move);
         other_move_it != std::cend(proposed_move); other_move_it++) {
      if (other_move_it != move_it && other_move_it->post == move_it->post) {
        invalid = true;
        break;
      }
    }
    if (invalid) {
      continue;
    }

    auto actual_it = elve_pos.find(move_it->pre);
    assert(actual_it != std::cend(elve_pos));
    elve_pos.erase(actual_it);
    elve_pos.emplace(move_it->post);
    elve_move = true;
  }
  return elve_move;
}

ll solve1(input_t const &aInputData) {
  auto const neigborgs = get_neiborgs();
  auto elve_pos = aInputData;

  auto const comp_x = [](auto const &lhs, auto const &rhs) {
    return lhs.x < rhs.x;
  };
  auto const comp_y = [](auto const &lhs, auto const &rhs) {
    return lhs.y < rhs.y;
  };

  for (ll round = 0; round < 10; round++) {
    simulate_round(elve_pos, neigborgs, round);
  }

  auto const max_x =
      std::max_element(std::cbegin(elve_pos), std::cend(elve_pos), comp_x)->x;
  auto const min_x =
      std::min_element(std::cbegin(elve_pos), std::cend(elve_pos), comp_x)->x;

  auto const max_y =
      std::max_element(std::cbegin(elve_pos), std::cend(elve_pos), comp_y)->y;
  auto const min_y =
      std::min_element(std::cbegin(elve_pos), std::cend(elve_pos), comp_y)->y;

  // print_map(min_x, max_x, min_y, max_y, elve_pos);

  auto const result =
      (max_x - min_x + 1) * (max_y - min_y + 1) - elve_pos.size();

  return result;
}

ll solve2(input_t const &aInputData) {
  auto const neigborgs = get_neiborgs();
  auto elve_pos = aInputData;

  for (ll round = 0;; round++) {
    auto const elve_move = simulate_round(elve_pos, neigborgs, round);
    if (!elve_move) {
      return round + 1;
    }
  }

  return -1;
}

int main() {
  auto const inputData = readInput();

  // std::for_each(std::cbegin(inputData), std::cend(inputData),
  //               [](auto const &elem) {
  //                 std::cout << elem.first.x << "," << elem.first.y <<
  //                 std::endl;
  //               });
  // std::cout << std::endl;

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}