#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>

using ll = int64_t;

enum class Direction { Right = 0, Down, Left, Up };

struct Movement {
  size_t id;
  ll size;
  Direction dir;

  bool operator==(Movement const &other) const {
    return std::tie(id, size, dir) == std::tie(other.id, other.size, other.dir);
  }
};

struct Point {
  ll x;
  ll y;

  bool operator<(Point const &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
  bool operator==(Point const &other) const {
    return std::tie(x, y) == std::tie(other.x, other.y);
  }
};

struct Position {
  Point p;
  Direction d;

  bool operator==(Position const &other) const {
    return std::tie(p, d) == std::tie(other.p, other.d);
  }
};

using map_t = std::vector<std::vector<char>>;

struct InputData {
  Point start;
  map_t map;
  ll x_size;
  ll y_size;
  std::vector<Movement> movements;
};

using input_t = InputData;

input_t readInput() {
  input_t inputValues;

  std::string line;
  size_t max_size = 0;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      break;
    }

    inputValues.map.push_back(std::vector<char>{});
    std::transform(std::cbegin(line), std::cend(line),
                   std::back_inserter(inputValues.map.back()),
                   [](auto const &cell) { return cell; });
    max_size = std::max(max_size, inputValues.map.back().size());
  }

  std::for_each(std::begin(inputValues.map), std::end(inputValues.map),
                [=](auto &row) { row.resize(max_size, ' '); });

  inputValues.x_size = static_cast<ll>(inputValues.map.size());
  inputValues.y_size = max_size;

  auto const start_it = std::find(std::cbegin(inputValues.map.front()),
                                  std::cend(inputValues.map.front()), '.');
  inputValues.start.y =
      std::distance(std::cbegin(inputValues.map.front()), start_it);
  inputValues.start.x = 0;

  std::getline(std::cin, line);

  ll previous_pos = 0;
  for (size_t i = 0; i < line.size(); i++) {
    switch (line[i]) {
    case 'L': {
      auto const size = std::stoi(line.substr(previous_pos, i - previous_pos));
      previous_pos = i + 1;
      inputValues.movements.push_back(
          Movement{inputValues.movements.size(), size, Direction::Left});
      break;
    }
    case 'R': {
      auto const size = std::stoi(line.substr(previous_pos, i - previous_pos));
      previous_pos = i + 1;
      inputValues.movements.push_back(
          Movement{inputValues.movements.size(), size, Direction::Right});
      break;
    }
    default:
      break;
    }
  }

  auto const size = std::stoi(line.substr(previous_pos));
  inputValues.movements.push_back(
      Movement{inputValues.movements.size(), size, Direction::Right});

  return inputValues;
}

Direction rotate(Direction const &facing, Direction const &rotate) {
  switch (rotate) {
  case Direction::Right:
    return static_cast<Direction>((static_cast<int>(facing) + 1) % 4);
  case Direction::Left:
    return static_cast<Direction>((static_cast<int>(facing) + 3) % 4);
  default:
    assert(false);
    return Direction::Right;
  }
}

Point transport(Point const &original, Point const &actual, map_t const &map) {
  if (original.x == actual.x) {
    if (actual.y > original.y) {
      auto const pos =
          std::find_if(std::cbegin(map[original.x]), std::cend(map[original.x]),
                       [](auto const &cell) { return cell != ' '; });
      if (*pos == '#')
        return original;
      else {
        return Point{original.x,
                     std::distance(std::cbegin(map[original.x]), pos)};
      }
    } else {
      auto const pos = std::find_if(
          std::crbegin(map[original.x]), std::crend(map[original.x]),
          [](auto const &cell) { return cell != ' '; });
      if (*pos == '#')
        return original;
      else {
        return Point{original.x,
                     std::distance(std::cbegin(map[original.x]), pos.base()) -
                         1};
      }
    }
  } else {
    if (actual.x > original.x) {
      auto const pos =
          std::find_if(std::cbegin(map), std::cend(map),
                       [&](auto const &row) { return row[original.y] != ' '; });
      if ((*pos)[original.y] == '#')
        return original;
      else {
        return Point{std::distance(std::cbegin(map), pos), original.y};
      }
    } else {
      auto const pos =
          std::find_if(std::crbegin(map), std::crend(map),
                       [&](auto const &row) { return row[original.y] != ' '; });
      if ((*pos)[original.y] == '#')
        return original;
      else {
        return Point{std::distance(std::cbegin(map), pos.base()) - 1,
                     original.y};
      }
    }
  }
}

Position move(Position const &pos, Movement const &movement, map_t const &map,
              ll y_size, bool last) {
  std::vector<Point> direction{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

  auto const &dir_move = direction[static_cast<int>(pos.d)];
  auto size = movement.size;
  auto np = pos;
  for (int i = 0; i < size; i++) {
    auto const npm = Point{np.p.x + dir_move.x, np.p.y + dir_move.y};
    if (0 <= npm.x && npm.x < static_cast<int>(map.size()) && 0 <= npm.y &&
        npm.y < y_size) {
      if (map[npm.x][npm.y] == '.') {
        np.p = npm;
      } else if (map[npm.x][npm.y] == '#') {
        break;
      } else if (map[npm.x][npm.y] == ' ') {
        np.p = transport(np.p, npm, map);
      }
    } else if (0 <= npm.x && npm.x < static_cast<int>(map.size())) {
      np.p = transport(np.p, npm, map);
    } else if (0 <= npm.y && npm.y < y_size) {
      np.p = transport(np.p, npm, map);
    } else {
      assert(false);
    }
  }

  if (!last) {
    np.d = rotate(np.d, movement.dir);
  }
  return np;
}

ll solve1(input_t const &aInputData) {
  auto const final_pos = std::accumulate(
      std::cbegin(aInputData.movements), std::cend(aInputData.movements),
      Position{aInputData.start, Direction::Right},
      [&](auto const &start_pos, auto const &movement) {
        return move(start_pos, movement, aInputData.map, aInputData.y_size,
                    movement == aInputData.movements.back());
      });

  return 1000 * (final_pos.p.x + 1) + 4 * (final_pos.p.y + 1) +
         static_cast<int>(final_pos.d);
}

void print(Direction const &dir) {
  switch (dir) {
  case Direction::Right:
    std::cout << "Right";
    break;
  case Direction::Down:
    std::cout << "Down";
    break;
  case Direction::Left:
    std::cout << "Left";
    break;
  case Direction::Up:
    std::cout << "Up";
    break;
  }
}

enum Going { Normal, Reverse };

struct CubeMove {
  Point cube_id{0, 0};
  Direction dir{Direction::Right};
  Going goX{Going::Normal};
  Going goY{Going::Normal};
};

std::map<Point, std::vector<CubeMove>> initialize_mapping_real() {
  std::map<Point, std::vector<CubeMove>> mapping;
  mapping.emplace(Point{0, 1},
                  std::vector<CubeMove>{
                      CubeMove{Point{}, {}}, CubeMove{Point{}, {}},
                      CubeMove{Point{2, 0}, Direction::Right, Going::Reverse},
                      CubeMove{Point{3, 0}, Direction::Right}});
  mapping.emplace(
      Point{0, 2},
      std::vector<CubeMove>{
          CubeMove{Point{2, 1}, Direction::Left, Going::Reverse,
                   Going::Reverse},
          CubeMove{Point{1, 1}, Direction::Left, Going::Normal, Going::Reverse},
          CubeMove{Point{}, {}},
          CubeMove{Point{3, 0}, Direction::Up, Going::Reverse}});
  mapping.emplace(Point{1, 1},
                  std::vector<CubeMove>{
                      CubeMove{Point{0, 2}, Direction::Up, Going::Reverse},
                      CubeMove{Point{}, {}},
                      CubeMove{Point{2, 0}, Direction::Down},
                      CubeMove{Point{}, {}}});
  mapping.emplace(Point{2, 0},
                  std::vector<CubeMove>{
                      CubeMove{Point{}, {}}, CubeMove{Point{}, {}},
                      CubeMove{Point{0, 1}, Direction::Right, Going::Reverse},
                      CubeMove{Point{1, 1}, Direction::Right}});
  mapping.emplace(
      Point{2, 1},
      std::vector<CubeMove>{
          CubeMove{Point{0, 2}, Direction::Left, Going::Reverse,
                   Going::Reverse},
          CubeMove{Point{3, 0}, Direction::Left, Going::Normal, Going::Reverse},
          CubeMove{Point{}, {}}, CubeMove{Point{}, {}}});
  mapping.emplace(Point{3, 0},
                  std::vector<CubeMove>{
                      CubeMove{Point{2, 1}, Direction::Up, Going::Reverse},
                      CubeMove{Point{0, 2}, Direction::Down},
                      CubeMove{Point{0, 1}, Direction::Down},
                      CubeMove{Point{}, {}}});
  return mapping;
}

std::map<Point, std::vector<CubeMove>> initialize_mapping_test() {
  std::map<Point, std::vector<CubeMove>> mapping;
  mapping.emplace(Point{0, 2},
                  std::vector<CubeMove>{
                      CubeMove{Point{2, 3}, Direction::Left, Going::Reverse,
                               Going::Reverse},
                      CubeMove{}, CubeMove{Point{1, 1}, Direction::Down},
                      CubeMove{Point{1, 0}, Direction::Down, Going::Reverse}});
  mapping.emplace(
      Point{1, 0},
      std::vector<CubeMove>{
          CubeMove{},
          CubeMove{Point{2, 2}, Direction::Up, Going::Reverse, Going::Reverse},
          CubeMove{Point{2, 3}, Direction::Up, Going::Reverse},
          CubeMove{Point{0, 2}, Direction::Down, Going::Reverse}});
  mapping.emplace(Point{1, 1},
                  std::vector<CubeMove>{
                      CubeMove{},
                      CubeMove{Point{2, 2}, Direction::Right, Going::Reverse},
                      CubeMove{}, CubeMove{Point{0, 2}, Direction::Right}});
  mapping.emplace(
      Point{1, 2},
      std::vector<CubeMove>{
          CubeMove{Point{2, 3}, Direction::Down, Going::Normal, Going::Reverse},
          CubeMove{Point{}, {}}, CubeMove{Point{}, {}}, CubeMove{Point{}, {}}});
  mapping.emplace(
      Point{2, 2},
      std::vector<CubeMove>{
          CubeMove{Point{}, {}},
          CubeMove{Point{1, 0}, Direction::Up, Going::Reverse, Going::Reverse},
          CubeMove{Point{1, 1}, Direction::Up, Going::Reverse},
          CubeMove{Point{}, {}}});
  mapping.emplace(Point{2, 2},
                  std::vector<CubeMove>{
                      CubeMove{Point{0, 2}, Direction::Left, Going::Reverse},
                      CubeMove{Point{1, 0}, Direction::Right, Going::Reverse},
                      CubeMove{Point{}, {}},
                      CubeMove{Point{1, 2}, Direction::Left, Going::Reverse}});
  return mapping;
}

Position transport2(Position const &pos, map_t const &map) {
  auto const mapping = initialize_mapping_real();
  // auto const mapping = initialize_mapping_test();

  auto const face_size =
      static_cast<ll>(std::max(map.size(), map.front().size()) / 4);
  auto const face_x = pos.p.x / face_size;
  auto const face_y = pos.p.y / face_size;

  auto const map_it = mapping.find({face_x, face_y});
  assert(map_it != std::cend(mapping));

  auto const &moving = map_it->second[static_cast<ll>(pos.d)];

  auto const new_x = moving.cube_id.x * face_size;
  auto const new_y = moving.cube_id.y * face_size;

  auto diff_x = pos.p.x - face_x * face_size;
  auto diff_y = pos.p.y - face_y * face_size;
  assert(0 <= diff_x && diff_x < face_size);
  assert(0 <= diff_y && diff_y < face_size);

  if (static_cast<ll>(pos.d) % 2 != static_cast<ll>(moving.dir) % 2) {
    std::swap(diff_x, diff_y);
  }

  // std::cout << "  " << face_x << "," << face_y << " -> " << moving.cube_id.x
  //           << "," << moving.cube_id.y << std::endl;

  switch (moving.dir) {
  case Direction::Right: {
    auto const xx =
        new_x +
        ((moving.goX == Going::Normal) ? diff_x : (face_size - diff_x - 1));
    auto const yy =
        (moving.goY == Going::Normal) ? new_y : (new_y + face_size - 1);
    auto const np = Point{xx, yy};
    return (map[np.x][np.y] == '#') ? pos : Position{np, Direction::Right};
  }
  case Direction::Down: {
    auto const xx =
        (moving.goX == Going::Normal) ? new_x : (new_x + face_size - 1);
    auto const yy =
        new_y +
        ((moving.goY == Going::Normal) ? diff_y : (face_size - diff_y - 1));
    auto const np = Point{xx, yy};
    return (map[np.x][np.y] == '#') ? pos : Position{np, Direction::Down};
  }
  case Direction::Left: {
    auto const xx =
        new_x +
        ((moving.goX == Going::Normal) ? diff_x : (face_size - diff_x - 1));
    auto const yy =
        (moving.goY == Going::Normal) ? new_y : (new_y + face_size - 1);
    auto const np = Point{xx, yy};
    return (map[np.x][np.y] == '#') ? pos : Position{np, Direction::Left};
  }
  case Direction::Up: {
    auto const xx =
        (moving.goX == Going::Normal) ? new_x : (new_x + face_size - 1);
    auto const yy =
        new_y +
        ((moving.goY == Going::Normal) ? diff_y : (face_size - diff_y - 1));
    auto const np = Point{xx, yy};
    return (map[np.x][np.y] == '#') ? pos : Position{np, Direction::Up};
  }
  }
  assert(false);
  return Position{};
}

Position move2(Position const &pos, Movement const &movement, map_t const &map,
               ll y_size, bool last) {
  std::vector<Point> direction{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

  auto size = movement.size;
  auto np = pos;
  for (int i = 0; i < size; i++) {
    auto const &dir_move = direction[static_cast<int>(np.d)];
    auto const npm = Point{np.p.x + dir_move.x, np.p.y + dir_move.y};
    if (0 <= npm.x && npm.x < static_cast<int>(map.size()) && 0 <= npm.y &&
        npm.y < y_size) {
      if (map[npm.x][npm.y] == '.') {
        np.p = npm;
      } else if (map[npm.x][npm.y] == '#') {
        break;
      } else if (map[npm.x][npm.y] == ' ') {
        auto const nnp = transport2(np, map);
        if (!(nnp == np)) {
          // std::cout << "  cross pre  " << np.p.x + 1 << "," << np.p.y + 1
          //           << ", dir: ";
          // print(np.d);
          // std::cout << std::endl;
          // std::cout << "  cross post " << nnp.p.x + 1 << "," << nnp.p.y + 1
          //           << ", dir: ";
          // print(nnp.d);
          // std::cout << std::endl;

          np = nnp;
        } else {
          break;
        }
      }
    } else if (0 <= npm.x && npm.x < static_cast<int>(map.size())) {
      auto const nnp = transport2(np, map);
      if (!(nnp == np)) {
        // std::cout << "  cross pre  " << np.p.x + 1 << "," << np.p.y + 1
        //           << ", dir: ";
        // print(np.d);
        // std::cout << std::endl;
        // std::cout << "  cross post " << nnp.p.x + 1 << "," << nnp.p.y + 1
        //           << ", dir: ";
        // print(nnp.d);
        // std::cout << std::endl;

        np = nnp;
      } else {
        break;
      }
    } else if (0 <= npm.y && npm.y < y_size) {
      auto const nnp = transport2(np, map);
      if (!(nnp == np)) {
        // std::cout << "  cross pre  " << np.p.x + 1 << "," << np.p.y + 1
        //           << ", dir: ";
        // print(np.d);
        // std::cout << std::endl;
        // std::cout << "  cross post " << nnp.p.x + 1 << "," << nnp.p.y + 1
        //           << ", dir: ";
        // print(nnp.d);
        // std::cout << std::endl;

        np = nnp;
      } else {
        break;
      }
    } else {
      assert(false);
    }
  }

  if (!last) {
    np.d = rotate(np.d, movement.dir);
  }

  // std::cout << "= move: " << movement.id << ", size: " << movement.size
  //           << ", dir: ";
  // print(movement.dir);
  // std::cout << std::endl;
  // std::cout << "new pos: " << np.p.x + 1 << "," << np.p.y + 1 << ", dir: ";
  // print(np.d);
  // std::cout << std::endl;

  return np;
}

ll solve2(input_t const &aInputData) {
  auto const final_pos = std::accumulate(
      std::cbegin(aInputData.movements), std::cend(aInputData.movements),
      Position{aInputData.start, Direction::Right},
      [&](auto const &start_pos, auto const &movement) {
        return move2(start_pos, movement, aInputData.map, aInputData.y_size,
                     movement == aInputData.movements.back());
      });

  return 1000 * (final_pos.p.x + 1) + 4 * (final_pos.p.y + 1) +
         static_cast<int>(final_pos.d);
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}