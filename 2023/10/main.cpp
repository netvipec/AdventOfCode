#include <bits/stdc++.h>
#include <vector>

using ll = int64_t;

struct Pos {
  ll row;
  ll col;

  bool operator<(const Pos &other) const {
    return std::tie(row, col) < std::tie(other.row, other.col);
  }
  bool operator==(const Pos &other) const {
    return std::tie(row, col) == std::tie(other.row, other.col);
  }
};

using grid_t = std::vector<std::vector<char>>;

struct Input {
  grid_t grid;
  Pos start;

  ll maxRow;
  ll maxCol;
};

struct NodeData {
  Pos pos;
  ll dir;
  ll length;
};

const std::map<char, std::vector<Pos>> kPipes{
    {'|', std::vector<Pos>{Pos{-1, 0}, Pos{1, 0}}},
    {'|', std::vector<Pos>{Pos{-1, 0}, Pos{1, 0}}},
    {'-', std::vector<Pos>{Pos{0, -1}, Pos{0, 1}}},
    {'L', std::vector<Pos>{Pos{-1, 0}, Pos{0, 1}}},
    {'J', std::vector<Pos>{Pos{-1, 0}, Pos{0, -1}}},
    {'7', std::vector<Pos>{Pos{0, -1}, Pos{1, 0}}},
    {'F', std::vector<Pos>{Pos{0, 1}, Pos{1, 0}}}};

const std::vector<Pos> kMoves{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

using input_t = Input;

Pos find_start(grid_t const &aInputData) {
  for (size_t i = 0; i < aInputData.size(); i++) {
    const auto &row = aInputData[i];
    for (size_t j = 0; j < aInputData[i].size(); j++) {
      if (row[j] == 'S') {
        return Pos{static_cast<ll>(i), static_cast<ll>(j)};
      }
    }
  }
  assert(false);
  return Pos{0, 0};
}

std::vector<Pos> get_neighbors_start(input_t const &inputData) {
  std::vector<Pos> neighbors_start;
  for (auto const &move : kMoves) {
    auto const new_row = inputData.start.row + move.row;
    auto const new_col = inputData.start.col + move.col;
    if (new_row < 0 || new_row >= inputData.maxRow || new_col < 0 ||
        new_col >= inputData.maxCol) {
      continue;
    }
    if (inputData.grid[new_row][new_col] == '.') {
      continue;
    }

    auto const &neighbors = kPipes.find(inputData.grid[new_row][new_col]);
    assert(neighbors != std::cend(kPipes));
    if (std::any_of(std::cbegin(neighbors->second),
                    std::cend(neighbors->second), [&](const auto &neighbor) {
                      auto const n_row = new_row + neighbor.row;
                      auto const n_col = new_col + neighbor.col;

                      return n_row == inputData.start.row &&
                             n_col == inputData.start.col;
                    })) {
      neighbors_start.push_back(move);
    }
  }

  std::sort(std::begin(neighbors_start), std::end(neighbors_start));
  assert(neighbors_start.size() == 2);
  return neighbors_start;
}

void Print(Pos const &pos) {
  std::cout << pos.row << "," << pos.col << std::endl;
}
void Print(grid_t const &grid) {
  std::for_each(std::cbegin(grid), std::cend(grid), [](auto const &row) {
    std::for_each(std::cbegin(row), std::cend(row),
                  [](auto const &elem) { std::cout << elem; });
    std::cout << std::endl;
  });
  std::cout << std::endl;
}

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues.grid.push_back(std::vector<char>{});
    std::transform(std::cbegin(line), std::cend(line),
                   std::back_inserter(inputValues.grid.back()),
                   [](const auto &elem) { return elem; });
  }

  inputValues.maxRow = inputValues.grid.size();
  inputValues.maxCol = inputValues.grid.front().size();
  inputValues.start = find_start(inputValues.grid);
  auto const neighbors_start = get_neighbors_start(inputValues);
  auto const start_pipe_it = std::find_if(
      std::cbegin(kPipes), std::cend(kPipes),
      [&](auto const &pipe) { return pipe.second == neighbors_start; });
  assert(start_pipe_it != std::cend(kPipes));
  inputValues.grid[inputValues.start.row][inputValues.start.col] =
      start_pipe_it->first;

  return inputValues;
}

std::set<Pos> get_loop(input_t const &aInputData) {
  std::set<Pos> seen;

  std::deque<Pos> nodes;
  nodes.push_back(aInputData.start);
  while (!nodes.empty()) {
    const auto top = nodes.front();
    nodes.pop_front();

    auto const &neighbors = kPipes.find(aInputData.grid[top.row][top.col]);
    assert(neighbors != std::cend(kPipes));

    for (auto const &neighbor : neighbors->second) {
      auto const n_row = top.row + neighbor.row;
      auto const n_col = top.col + neighbor.col;

      assert(!(n_row < 0 || n_row >= aInputData.maxRow || n_col < 0 ||
               n_col >= aInputData.maxCol));
      assert(aInputData.grid[n_row][n_col] != '.');

      Pos node{n_row, n_col};
      if (seen.find(node) != std::cend(seen)) {
        continue;
      }

      nodes.push_back(node);
      seen.insert(node);
    }
  }
  return seen;
}

ll solve1(input_t const &aInputData) {
  auto const loop = get_loop(aInputData);

  return loop.size() / 2;
}

template <char Divider, char One, char Two, char Three, char Four>
bool get_inside(Pos const &p, grid_t const &grid, Pos const &move) {
  ll counter = 0;
  ll dir = 0;
  auto pp = p;
  for (;;) {
    pp.row += move.row;
    pp.col += move.col;
    if (pp.row < 0 || pp.row >= static_cast<ll>(grid.size()) || pp.col < 0 ||
        pp.col >= static_cast<ll>(grid.front().size())) {
      break;
    }

    switch (grid[pp.row][pp.col]) {
    case Divider:
      counter++;
      break;
    case One:
    case Two:
      if (dir < 0) {
        counter++;
        dir = 0;
      } else if (dir > 0) {
        dir = 0;
      } else {
        dir = 1;
      }
      break;
    case Three:
    case Four:
      if (dir > 0) {
        counter++;
        dir = 0;
      } else if (dir < 0) {
        dir = 0;
      } else {
        dir = -1;
      }
      break;
    }
  }
  return counter % 2 == 1;
}

ll solve2(input_t const &aInputData) {
  auto const loop = get_loop(aInputData);

  auto grid = aInputData.grid;
  for (ll i = 0; i < aInputData.maxRow; i++) {
    for (ll j = 0; j < aInputData.maxCol; j++) {
      auto const it = loop.find(Pos{i, j});
      if (it != std::cend(loop)) {
        continue;
      }

      grid[i][j] = '.';
    }
  }

  std::cout << "Start: ";
  Print(aInputData.start);
  std::cout << "Size: " << aInputData.maxRow << "," << aInputData.maxCol
            << std::endl;
  Print(grid);

  ll count = 0;
  for (ll i = 0; i < aInputData.maxRow; i++) {
    for (ll j = 0; j < aInputData.maxCol; j++) {
      if (grid[i][j] != '.') {
        continue;
      }
      auto is_inside =
          get_inside<'|', '7', 'F', 'J', 'L'>(Pos{i, j}, grid, {0, -1}) &&
          get_inside<'|', '7', 'F', 'J', 'L'>(Pos{i, j}, grid, {0, 1}) &&
          get_inside<'-', 'F', 'L', '7', 'J'>(Pos{i, j}, grid, {-1, 0}) &&
          get_inside<'-', 'F', 'L', '7', 'J'>(Pos{i, j}, grid, {1, 0});
      if (is_inside) {
        Print(Pos{i, j});
        count++;
      }
    }
  }

  return count;
}

int main() {
  auto const inputData = readInput();

  // std::cout << "Start: ";
  // Print(inputData.start);
  // Print(inputData.grid);

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}