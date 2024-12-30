#include <algorithm>
#include <bits/stdc++.h>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <queue>
#include <utility>

using ll = int64_t;

struct Point {
  int8_t x;
  int8_t y;

  bool operator<(Point const &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
  bool operator==(Point const &other) const {
    return std::tie(x, y) == std::tie(other.x, other.y);
  }
};

enum class Direction { Right, Down, Left, Up };
const std::vector<Point> DIR{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

using winds_t = std::map<Point, std::vector<Direction>>;

struct InputData {
  std::vector<std::string> map;
  winds_t wind;
  Point start_pos;
  Point end_pos;
};

using input_t = InputData;

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues.map.push_back(line);
  }

  auto const start_it = std::find(std::cbegin(inputValues.map.front()),
                                  std::cend(inputValues.map.front()), '.');
  auto const end_it = std::find(std::cbegin(inputValues.map.back()),
                                std::cend(inputValues.map.back()), '.');
  inputValues.start_pos.x = 0;
  inputValues.start_pos.y =
      std::distance(std::cbegin(inputValues.map.front()), start_it);
  inputValues.end_pos.x = inputValues.map.size() - 1;
  inputValues.end_pos.y =
      std::distance(std::cbegin(inputValues.map.back()), end_it);

  for (int8_t x = 1; x < inputValues.map.size() - 1; x++) {
    for (int8_t y = 1; y < inputValues.map[x].size() - 1; y++) {
      if (inputValues.map[x][y] != '.') {
        switch (inputValues.map[x][y]) {
        case '>':
          inputValues.wind[Point{x, y}].push_back(Direction::Right);
          break;
        case 'v':
          inputValues.wind[Point{x, y}].push_back(Direction::Down);
          break;
        case '<':
          inputValues.wind[Point{x, y}].push_back(Direction::Left);
          break;
        case '^':
          inputValues.wind[Point{x, y}].push_back(Direction::Up);
          break;
        default:
          assert(false);
        }
      }
    }
  }

  return inputValues;
}

struct DFSData {
  Point pos;
  uint16_t steps;
  uint16_t distance_to_end;

  bool operator>(DFSData const &other) const {
    return distance_to_end + steps > other.distance_to_end + other.steps;
  }
};

winds_t move_winds(winds_t const &winds, ll max_x, ll max_y) {
  winds_t result;

  std::for_each(std::cbegin(winds), std::cend(winds), [&](auto const &wind) {
    std::for_each(
        std::cbegin(wind.second), std::cend(wind.second), [&](auto const &dir) {
          auto const &dir_move = DIR[static_cast<ll>(dir)];
          auto new_wind = Point{static_cast<int8_t>(wind.first.x + dir_move.x),
                                static_cast<int8_t>(wind.first.y + dir_move.y)};
          if (new_wind.x < 1) {
            new_wind.x = max_x - 2;
          } else if (new_wind.x >= max_x - 1) {
            new_wind.x = 1;
          }
          if (new_wind.y < 1) {
            new_wind.y = max_y - 2;
          } else if (new_wind.y >= max_y - 1) {
            new_wind.y = 1;
          }

          result[new_wind].push_back(dir);
        });
  });

  // std::for_each(std::cbegin(result), std::cend(result), [](auto const &pos) {
  //   std::cout << pos.first.x << "," << pos.first.y << ":";
  //   std::for_each(std::cbegin(pos.second), std::cend(pos.second),
  //                 [](auto const &dir) {
  //                   switch (dir) {
  //                   case Direction::Right:
  //                     std::cout << "R,";
  //                     break;
  //                   case Direction::Down:
  //                     std::cout << "D,";
  //                     break;
  //                   case Direction::Left:
  //                     std::cout << "L,";
  //                     break;
  //                   case Direction::Up:
  //                     std::cout << "U,";
  //                     break;
  //                   }
  //                 });
  //   std::cout << std::endl;
  // });

  return result;
}

uint16_t distance(Point const &p1, Point const &p2) {
  return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

char direction_str(Direction const &dir) {
  switch (dir) {
  case Direction::Right:
    return '>';
  case Direction::Down:
    return 'v';
  case Direction::Left:
    return '<';
  case Direction::Up:
    return '^';
  }
  return 'X';
}

void print(Point const &p, winds_t const &winds,
           std::vector<std::string> const &map) {
  auto new_map = map;
  for (ll x = 1; x < new_map.size() - 1; x++) {
    for (ll y = 1; y < new_map[x].size() - 1; y++) {
      new_map[x][y] = '.';
    }
  }

  std::for_each(std::cbegin(winds), std::cend(winds), [&](auto const &wind) {
    if (wind.second.size() > 1) {
      new_map[wind.first.x][wind.first.y] = '0' + wind.second.size();
    } else {
      new_map[wind.first.x][wind.first.y] = direction_str(wind.second.front());
    }
  });

  assert(new_map[p.x][p.y] == '.');
  new_map[p.x][p.y] = 'E';

  std::for_each(std::cbegin(new_map), std::cend(new_map),
                [](auto const &row) { std::cout << row << std::endl; });
  std::cout << std::endl;
}

ll bfs(Point const &start_pos, Point const &end_pos,
       std::vector<winds_t> const &winds, std::vector<std::string> const &map,
       uint16_t steps = 0) {
  ll best_until_now = std::numeric_limits<ll>::max();
  std::priority_queue<DFSData, std::vector<DFSData>, std::greater<>> process;
  process.push(DFSData{start_pos, steps, distance(start_pos, end_pos)});

  std::set<std::pair<Point, uint16_t>> visited;

  while (!process.empty()) {
    DFSData p = process.top();
    process.pop();

    // std::cout << p.pos.x << "," << p.pos.y << std::endl;

    if (p.pos == end_pos) {
      return p.steps;
    }

    auto const visited_pair = visited.insert({p.pos, p.steps % winds.size()});
    if (!visited_pair.second) {
      continue;
    }

    if (p.steps + p.distance_to_end > best_until_now) {
      continue;
    }

    // print(p.pos, winds[p.steps], aInputData.map);

    auto const &new_winds = winds[(p.steps + 1) % winds.size()];

    std::for_each(std::cbegin(DIR), std::cend(DIR), [&](auto const &dir) {
      auto new_pos = Point{static_cast<int8_t>(p.pos.x + dir.x),
                           static_cast<int8_t>(p.pos.y + dir.y)};
      if (0 <= new_pos.x && new_pos.x < map.size() && 0 <= new_pos.y &&
          new_pos.y < map.front().size() && map[new_pos.x][new_pos.y] != '#') {
        auto const place_it = new_winds.find(new_pos);
        if (place_it == std::cend(new_winds)) {
          if (new_pos == end_pos && p.steps + 1 < best_until_now) {
            best_until_now = p.steps + 1;
            std::cout << "best: " << best_until_now << std::endl;
          }
          process.push(DFSData{new_pos, static_cast<uint16_t>(p.steps + 1),
                               distance(new_pos, end_pos)});
        }
      }
    });

    auto const same_place_it = new_winds.find(p.pos);
    if (same_place_it == std::cend(new_winds)) {
      process.push(DFSData{p.pos, static_cast<uint16_t>(p.steps + 1),
                           p.distance_to_end});
    }
  }

  return -1;
}

ll solve1(input_t const &aInputData) {
  std::vector<winds_t> winds{aInputData.wind};
  for (ll i = 0;; i++) {
    auto const new_winds = move_winds(winds.back(), aInputData.map.size(),
                                      aInputData.map.front().size());
    if (new_winds == winds.front()) {
      std::cout << i << std::endl;
      break;
    }
    winds.push_back(new_winds);

    // print(aInputData.start_pos, winds.back(), aInputData.map);
  }

  return bfs(aInputData.start_pos, aInputData.end_pos, winds, aInputData.map);
}

ll solve2(input_t const &aInputData) {
  std::vector<winds_t> winds{aInputData.wind};
  for (ll i = 0;; i++) {
    auto const new_winds = move_winds(winds.back(), aInputData.map.size(),
                                      aInputData.map.front().size());
    if (new_winds == winds.front()) {
      std::cout << i << std::endl;
      break;
    }
    winds.push_back(new_winds);

    // print(aInputData.start_pos, winds.back(), aInputData.map);
  }

  auto const steps_go1 =
      bfs(aInputData.start_pos, aInputData.end_pos, winds, aInputData.map);
  auto const steps_back1 = bfs(aInputData.end_pos, aInputData.start_pos, winds,
                               aInputData.map, steps_go1);
  auto const steps_go2 = bfs(aInputData.start_pos, aInputData.end_pos, winds,
                             aInputData.map, steps_back1);
  return steps_go2;
}

int main() {
  auto const inputData = readInput();

  std::for_each(std::cbegin(inputData.map), std::cend(inputData.map),
                [](auto const &elem) { std::cout << elem << std::endl; });
  std::cout << std::endl;

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}