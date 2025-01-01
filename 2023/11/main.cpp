#include <bits/stdc++.h>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <vector>

using ll = int64_t;
using input_t = std::vector<std::vector<bool>>;

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

const std::vector<Pos> kMoves{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues.push_back(std::vector<bool>{});
    std::transform(std::cbegin(line), std::cend(line),
                   std::back_inserter(inputValues.back()),
                   [](auto const &cell) { return cell == '#'; });
  }
  return inputValues;
}

std::pair<std::vector<ll>, std::vector<ll>>
get_empty(input_t const &aInputData) {
  std::vector<ll> rows;
  for (size_t i = 0; i < aInputData.size(); i++) {
    auto const is_empty_cells =
        std::count(std::cbegin(aInputData[i]), std::cend(aInputData[i]), false);
    if (is_empty_cells == static_cast<ll>(aInputData[i].size())) {
      rows.push_back(i);
    }
  }

  std::vector<ll> cols;
  for (size_t i = 0; i < aInputData.front().size(); i++) {
    auto const is_empty_cells =
        std::count_if(std::cbegin(aInputData), std::cend(aInputData),
                      [&](auto const &row) { return row[i] == false; });
    if (is_empty_cells == static_cast<ll>(aInputData[i].size())) {
      cols.push_back(i);
    }
  }

  return {rows, cols};
}

void Print(input_t const &grid) {
  std::for_each(std::cbegin(grid), std::cend(grid), [](auto const &row) {
    std::for_each(std::cbegin(row), std::cend(row),
                  [](auto const &cell) { std::cout << cell; });
    std::cout << std::endl;
  });
}

ll get_shortest_path(input_t const &aInputData, ll start,
                     std::vector<Pos> const &galaxies,
                     std::vector<ll> const &rows, std::vector<ll> const &cols,
                     ll expand_size) {
  auto const maxRow = static_cast<ll>(aInputData.size());
  auto const maxCol = static_cast<ll>(aInputData.front().size());

  std::vector<std::vector<bool>> seen(maxRow, std::vector<bool>(maxCol));
  seen[galaxies[start].row][galaxies[start].col] = true;

  ll path_accumulator = 0;
  std::priority_queue<std::pair<ll, Pos>, std::vector<std::pair<ll, Pos>>,
                      std::greater<std::pair<ll, Pos>>>
      nodes;
  nodes.push({0, galaxies[start]});
  std::vector<bool> galaxy_path_found(galaxies.size(), false);
  galaxy_path_found[start] = true;
  while (!nodes.empty()) {
    const auto top = nodes.top();
    nodes.pop();

    auto const galaxy_it =
        std::find(std::cbegin(galaxies), std::cend(galaxies), top.second);
    if (galaxy_it != std::cend(galaxies)) {
      auto const galaxy_index = std::distance(std::cbegin(galaxies), galaxy_it);
      if (!galaxy_path_found[galaxy_index]) {
        galaxy_path_found[galaxy_index] = true;
        path_accumulator += top.first;
        // std::cout << start + 1 << " -> " << galaxy_index + 1 << " = "
        //           << top.first << std::endl;
        if (std::all_of(
                std::cbegin(galaxy_path_found), std::cend(galaxy_path_found),
                [](auto const &galaxy_found) { return galaxy_found; })) {
          return path_accumulator;
        }
      }
    }

    for (auto const &move : kMoves) {
      auto const n_row = top.second.row + move.row;
      auto const n_col = top.second.col + move.col;

      if (n_row < 0 || n_row >= maxRow || n_col < 0 || n_col >= maxCol) {
        continue;
      }

      Pos node{n_row, n_col};
      if (seen[n_row][n_col]) {
        continue;
      }

      auto extra = 0;
      if (std::find(std::cbegin(rows), std::cend(rows), node.row) !=
          std::cend(rows)) {
        extra += expand_size;
      }
      if (std::find(std::cbegin(cols), std::cend(cols), node.col) !=
          std::cend(cols)) {
        extra += expand_size;
      }
      nodes.push({top.first + 1 + extra, node});
      seen[n_row][n_col] = true;
    }
  }
  assert(false);
  return 0;
}

std::vector<std::vector<ll>> comb(int N, int K) {
  std::string bitmask(K, 1); // K leading 1's
  bitmask.resize(N, 0);      // N-K trailing 0's

  std::vector<std::vector<ll>> result;
  // print integers and permute bitmask
  do {
    std::vector<ll> row;
    for (int i = 0; i < N; ++i) // [0..N-1] integers
    {
      if (bitmask[i]) {
        row.push_back(i);
      }
    }
    result.push_back(row);
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
  return result;
}

ll solve1(input_t const &aInputData) {
  auto const &grid = aInputData;
  auto const rowsCols = get_empty(aInputData);
  // Print(grid);
  std::vector<Pos> galaxies;
  for (ll row = 0; row < static_cast<ll>(grid.size()); row++) {
    for (ll col = 0; col < static_cast<ll>(grid.front().size()); col++) {
      if (grid[row][col]) {
        galaxies.push_back(Pos{row, col});
      }
    }
  }

  ll acc = 0;
  for (ll i = 0; i < static_cast<ll>(galaxies.size()); i++) {
    acc += get_shortest_path(grid, i, galaxies, rowsCols.first, rowsCols.second,
                             1);
  }
  return acc / 2;
}

ll solve1_fast(input_t const &aInputData) {
  auto const rowsCols = get_empty(aInputData);

  std::vector<Pos> galaxies;
  for (ll row = 0; row < static_cast<ll>(aInputData.size()); row++) {
    for (ll col = 0; col < static_cast<ll>(aInputData.front().size()); col++) {
      if (aInputData[row][col]) {
        galaxies.push_back(Pos{row, col});
      }
    }
  }

  auto const nChoose2 = comb(galaxies.size(), 2);

  return std::accumulate(
      std::cbegin(nChoose2), std::cend(nChoose2), 0ll,
      [&](auto const &base, auto const &elem) {
        std::vector<ll> row_pair{galaxies[elem.front()].row,
                                 galaxies[elem.back()].row};
        std::vector<ll> col_pair{galaxies[elem.front()].col,
                                 galaxies[elem.back()].col};
        std::sort(std::begin(row_pair), std::end(row_pair));
        std::sort(std::begin(col_pair), std::end(col_pair));
        auto const distance = row_pair.back() - row_pair.front() +
                              col_pair.back() - col_pair.front();
        auto const empty_rows = std::count_if(
            std::cbegin(rowsCols.first), std::cend(rowsCols.first),
            [&](auto const &row) {
              return row_pair.front() < row && row < row_pair.back();
            });
        auto const empty_cols = std::count_if(
            std::cbegin(rowsCols.second), std::cend(rowsCols.second),
            [&](auto const &col) {
              return col_pair.front() < col && col < col_pair.back();
            });
        return base + distance + empty_rows + empty_cols;
      });
}

ll solve2(input_t const &aInputData) {
  ll expand_size = 1000000 - 1;

  auto const &grid = aInputData;
  auto rows = std::vector<ll>{};
  auto cols = std::vector<ll>{};
  auto const rowsCols = get_empty(aInputData);
  // Print(grid);
  std::vector<Pos> galaxies;
  for (ll row = 0; row < static_cast<ll>(grid.size()); row++) {
    for (ll col = 0; col < static_cast<ll>(grid.front().size()); col++) {
      if (grid[row][col]) {
        galaxies.push_back(Pos{row, col});
      }
    }
  }

  ll acc = 0;
  for (ll i = 0; i < static_cast<ll>(galaxies.size()); i++) {
    acc += get_shortest_path(grid, i, galaxies, rowsCols.first, rowsCols.second,
                             expand_size);
  }
  return acc / 2;
}

ll solve2_fast(input_t const &aInputData) {
  ll expand_size = 1000000 - 1;
  auto const rowsCols = get_empty(aInputData);

  std::vector<Pos> galaxies;
  for (ll row = 0; row < static_cast<ll>(aInputData.size()); row++) {
    for (ll col = 0; col < static_cast<ll>(aInputData.front().size()); col++) {
      if (aInputData[row][col]) {
        galaxies.push_back(Pos{row, col});
      }
    }
  }

  auto const nChoose2 = comb(galaxies.size(), 2);

  return std::accumulate(
      std::cbegin(nChoose2), std::cend(nChoose2), 0ll,
      [&](auto const &base, auto const &elem) {
        std::vector<ll> row_pair{galaxies[elem.front()].row,
                                 galaxies[elem.back()].row};
        std::vector<ll> col_pair{galaxies[elem.front()].col,
                                 galaxies[elem.back()].col};
        std::sort(std::begin(row_pair), std::end(row_pair));
        std::sort(std::begin(col_pair), std::end(col_pair));
        auto const distance = row_pair.back() - row_pair.front() +
                              col_pair.back() - col_pair.front();
        auto const empty_rows = std::count_if(
            std::cbegin(rowsCols.first), std::cend(rowsCols.first),
            [&](auto const &row) {
              return row_pair.front() < row && row < row_pair.back();
            });
        auto const empty_cols = std::count_if(
            std::cbegin(rowsCols.second), std::cend(rowsCols.second),
            [&](auto const &col) {
              return col_pair.front() < col && col < col_pair.back();
            });
        return base + distance + empty_rows * expand_size +
               empty_cols * expand_size;
      });
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1_fast(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2_fast(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}