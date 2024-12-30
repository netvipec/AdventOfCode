#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <sys/types.h>
#include <utility>

using ll = int64_t;
using input_t = std::string;

constexpr ll BOARD_WIDTH = 7;

input_t readInput() {
  input_t inputValues;

  std::getline(std::cin, inputValues);
  return inputValues;
}

struct FallReturn {
  ll highest_rock = 0;
  ll jet_index = 0;
};

void print(std::vector<std::vector<char>> const &board) {
  std::for_each(std::crbegin(board), std::crend(board), [](auto const &row) {
    std::for_each(std::cbegin(row), std::cend(row),
                  [](auto const &cell) { std::cout << cell; });
    std::cout << std::endl;
  });
  std::cout << std::endl;
}

FallReturn fall_tetrominoes(std::vector<std::pair<ll, ll>> tetrominoe,
                            input_t const &input, ll jet_index,
                            std::deque<std::vector<char>> &board,
                            std::pair<ll, ll> pos) {
  bool stoped = false;
  do {
    switch (input[jet_index]) {
    case '>':
      if (std::none_of(std::cbegin(tetrominoe), std::cend(tetrominoe),
                       [&](auto const &p) {
                         auto const np = pos.second + p.second + 1;
                         return np >= BOARD_WIDTH ||
                                (pos.first + p.first <
                                     static_cast<ll>(board.size()) &&
                                 board[pos.first + p.first][np] != '.');
                       })) {
        pos.second++;
      }
      break;
    case '<':
      if (std::none_of(std::cbegin(tetrominoe), std::cend(tetrominoe),
                       [&](auto const &p) {
                         auto const np = pos.second + p.second - 1;
                         return np < 0 ||
                                (pos.first + p.first <
                                     static_cast<ll>(board.size()) &&
                                 board[pos.first + p.first][np] != '.');
                       })) {
        pos.second--;
      }
      break;
    default:
      assert(false);
      return {};
    }
    jet_index = (jet_index + 1) % input.size();

    if (pos.first > 0) {
      if (std::none_of(std::cbegin(tetrominoe), std::cend(tetrominoe),
                       [&](auto const &p) {
                         auto const np = pos.first + p.first - 1;
                         return !(np >= static_cast<ll>(board.size()) ||
                                  board[np][pos.second + p.second] == '.');
                       })) {
        pos.first--;
      } else {
        stoped = true;
      }
    } else {
      stoped = true;
    }
  } while (!stoped);

  while (std::any_of(std::cbegin(tetrominoe), std::cend(tetrominoe),
                     [&](auto const &p) {
                       auto const np = pos.first + p.first;
                       return np >= static_cast<ll>(board.size());
                     })) {
    board.push_back(std::vector<char>(BOARD_WIDTH, '.'));
  }
  std::for_each(std::cbegin(tetrominoe), std::cend(tetrominoe),
                [&](auto const &p) {
                  board[pos.first + p.first][pos.second + p.second] = '#';
                });

  auto const highest = std::distance(
      std::cbegin(board),
      std::find_if(std::crbegin(board), std::crend(board),
                   [](auto const &row) {
                     return std::find(std::cbegin(row), std::cend(row), '#') !=
                            std::cend(row);
                   })
          .base());

  // print(board);

  return {highest, jet_index};
}

ll solve1(input_t const &aInputData) {
  std::vector<std::vector<std::pair<ll, ll>>> tetrominoes{
      {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
      {{0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1}},
      {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}},
      {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
      {{0, 0}, {0, 1}, {1, 0}, {1, 1}}};

  std::deque<std::vector<char>> board;
  FallReturn return_rock{0, 0};
  for (int i = 0; i < 2022; i++) {
    auto const &tetrominoe = tetrominoes[i % tetrominoes.size()];

    std::pair<ll, ll> pos{return_rock.highest_rock + 3, 2};

    return_rock = fall_tetrominoes(tetrominoe, aInputData,
                                   return_rock.jet_index, board, pos);
  }
  return return_rock.highest_rock;
}

struct Comparator {
  bool operator()(std::deque<std::vector<char>> const &lhs,
                  std::deque<std::vector<char>> const &rhs) const {
    for (int i = 0; i < static_cast<int>(lhs.size()); i++) {
      if (lhs[i] < rhs[i]) {
        return true;
      }
    }
    return false;
  }
};

ll solve2(input_t const &aInputData) {
  std::vector<std::vector<std::pair<ll, ll>>> tetrominoes{
      {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
      {{0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1}},
      {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}},
      {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
      {{0, 0}, {0, 1}, {1, 0}, {1, 1}}};

  ll const limit = 5000;

  std::map<std::deque<std::vector<char>>, std::pair<ll, ll>, Comparator> cache;

  std::deque<std::vector<char>> board;
  FallReturn return_rock{0, 0};
  ll fallen_tetrominoes = 0;
  ll removed_lines = 0;
  std::pair<ll, ll> base;
  while (true) {
    auto const &tetrominoe =
        tetrominoes[fallen_tetrominoes % tetrominoes.size()];

    std::pair<ll, ll> pos{return_rock.highest_rock + 3, 2};

    return_rock = fall_tetrominoes(tetrominoe, aInputData,
                                   return_rock.jet_index, board, pos);
    if (board.size() > limit) {
      auto const to_remove = board.size() - limit;
      for (uint i = 0; i < to_remove; i++) {
        board.pop_front();
      }
      return_rock.highest_rock -= to_remove;
      removed_lines += to_remove;

      auto const pair = cache.emplace(
          board, std::make_pair(fallen_tetrominoes,
                                return_rock.highest_rock + removed_lines));
      if (!pair.second) {
        base = pair.first->second;
        break;
      }
    }
    fallen_tetrominoes++;
  }

  ll const repetition_size = fallen_tetrominoes - base.first;
  ll const target = 1000000000000ll;
  ll const repetition_count = (target - base.first) / repetition_size;
  ll const highest_rock_until_now =
      base.second + repetition_count * (removed_lines +
                                        return_rock.highest_rock - base.second);

  ll highest_rock_saved = return_rock.highest_rock;

  ll const previous = base.first + repetition_count * repetition_size;
  for (ll i = previous + 1; i < target; i++) {
    auto const &tetrominoe = tetrominoes[i % tetrominoes.size()];

    std::pair<ll, ll> pos{return_rock.highest_rock + 3, 2};

    return_rock = fall_tetrominoes(tetrominoe, aInputData,
                                   return_rock.jet_index, board, pos);
  }
  return highest_rock_until_now + return_rock.highest_rock - highest_rock_saved;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}