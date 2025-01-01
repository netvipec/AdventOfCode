#include <algorithm>
#include <bits/stdc++.h>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <utility>

using ll = int64_t;
using input_t = std::vector<std::vector<char>>;

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues.push_back(std::vector<char>());
    std::transform(std::cbegin(line), std::cend(line),
                   std::back_inserter(inputValues.back()),
                   [](auto const &cell) { return cell; });
  }
  return inputValues;
}

void print_grid(input_t const &aInputData) {
  std::for_each(std::cbegin(aInputData), std::cend(aInputData),
                [](auto const &row) {
                  std::for_each(std::cbegin(row), std::cend(row),
                                [](auto const &elem) { std::cout << elem; });
                  std::cout << std::endl;
                });
}

ll solve1(input_t const &aInputData) {
  ll sol = 0;
  for (ll col = 0; col < static_cast<ll>(aInputData.front().size()); col++) {
    ll last_block_row = -1;
    for (ll row = 0; row < static_cast<ll>(aInputData.size()); row++) {
      if (aInputData[row][col] == '#') {
        last_block_row = row;
      } else if (aInputData[row][col] == 'O') {
        sol += aInputData.size() - last_block_row - 1;
        last_block_row++;
      } else {
        assert(aInputData[row][col] == '.');
      }
    }
  }
  return sol;
}

void move_vertical(input_t &aInputData, ll dir) {
  static input_t grid = aInputData;

  for (ll c = 0; c < static_cast<ll>(aInputData.front().size()); c++) {
    ll col = dir > 0 ? c : aInputData.front().size() - c - 1;
    ll last_block_row = dir > 0 ? -1 : aInputData.size();
    for (ll r = 0; r < static_cast<ll>(aInputData.size()); r++) {
      ll row = dir > 0 ? r : aInputData.size() - r - 1;
      if (aInputData[row][col] == '#') {
        last_block_row = row;
        grid[row][col] = '#';
      } else if (aInputData[row][col] == 'O') {
        last_block_row += dir;
        grid[row][col] = '.';
        grid[last_block_row][col] = 'O';
      } else {
        assert(aInputData[row][col] == '.');
        grid[row][col] = '.';
      }
    }
  }
  aInputData.swap(grid);
}

void move_horizontal(input_t &aInputData, ll dir) {
  static input_t grid = aInputData;

  for (ll r = 0; r < static_cast<ll>(aInputData.size()); r++) {
    ll row = dir > 0 ? r : aInputData.size() - r - 1;
    ll last_block_col = dir > 0 ? -1 : aInputData.front().size();
    for (ll c = 0; c < static_cast<ll>(aInputData.front().size()); c++) {
      ll col = dir > 0 ? c : aInputData.front().size() - c - 1;
      if (aInputData[row][col] == '#') {
        last_block_col = col;
        grid[row][col] = '#';
      } else if (aInputData[row][col] == 'O') {
        last_block_col += dir;
        grid[row][col] = '.';
        grid[row][last_block_col] = 'O';
      } else {
        assert(aInputData[row][col] == '.');
        grid[row][col] = '.';
      }
    }
  }
  aInputData.swap(grid);
}

namespace my_utils {
namespace hash_impl {
namespace details {
namespace adl {
template <class T> std::size_t hash(T const &t) { return std::hash<T>{}(t); }
} // namespace adl
template <class T> std::size_t hasher(T const &t) {
  using adl::hash;
  return hash(t);
}
} // namespace details
struct hash_tag {};
template <class T> std::size_t hash(hash_tag, T const &t) {
  return details::hasher(t);
}
template <class T>
std::size_t hash_combine(hash_tag, std::size_t seed, T const &t) {
  seed ^= hash(hash_tag{}, t) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  return seed;
}
template <class Container>
std::size_t fash_hash_random_container(hash_tag, Container const &c) {
  std::size_t size = c.size();
  std::size_t stride = 1 + size / 10;
  std::size_t r = hash(hash_tag{}, size);
  for (std::size_t i = 0; i < size; i += stride) {
    r = hash_combine(hash_tag{}, r, c.data()[i]);
  }
  return r;
}
// std specializations go here:
template <class T, class A>
std::size_t hash(hash_tag, std::vector<T, A> const &v) {
  return fash_hash_random_container(hash_tag{}, v);
}
template <class T, std::size_t N>
std::size_t hash(hash_tag, std::array<T, N> const &a) {
  return fash_hash_random_container(hash_tag{}, a);
}
// etc
} // namespace hash_impl
struct my_hasher {
  template <class T> std::size_t operator()(T const &t) const {
    return hash_impl::hash(hash_impl::hash_tag{}, t);
  }
};
} // namespace my_utils

ll calculate_load(input_t const &grid) {
  ll sol = 0;
  for (ll row = 0; row < static_cast<ll>(grid.size()); row++) {
    for (ll col = 0; col < static_cast<ll>(grid.front().size()); col++) {
      if (grid[row][col] == 'O') {
        sol += grid.size() - row;
      }
    }
  }
  return sol;
}

ll solve2(input_t const &aInputData) {
  std::unordered_map<input_t, ll, my_utils::my_hasher> cache;
  ll max_cycles = 1000000000;

  auto grid = aInputData;
  ll idx_repetition = -1;
  for (ll cycles = 0; cycles < max_cycles; cycles++) {
    move_vertical(grid, 1);
    move_horizontal(grid, 1);
    move_vertical(grid, -1);
    move_horizontal(grid, -1);

    auto insert = cache.emplace(grid, cycles);
    if (!insert.second) {
      idx_repetition = insert.first->second;
      break;
    }
  }

  auto const remainder = (max_cycles - idx_repetition - 1) %
                         (static_cast<ll>(cache.size()) - idx_repetition);
  auto const idx = idx_repetition + remainder;
  auto const it =
      std::find_if(std::cbegin(cache), std::cend(cache),
                   [&](auto const &elem) { return elem.second == idx; });
  return calculate_load(it->first);
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}