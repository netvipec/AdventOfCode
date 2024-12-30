#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::string>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        input_values.emplace_back(line);
    }
    return input_values;
}

// clang-format off
const std::array<std::pair<ll, ll>, 8> D {{ {-1,-1}, {0,-1}, {1,-1},
                                            {-1,0},/*{0,0},*/{1,0},
                                            {-1,1},  {0,1},  {1,1} }};
// clang-format on

void evolve(input_t const& seat_layout,
            input_t& next_seat_layout,
            bool expanded_view = false,
            ll adjacent_occupied_min = 4) {
    for (ll i = 0, i_end = static_cast<ll>(seat_layout.size()); i < i_end; i++) {
        for (ll j = 0, j_end = static_cast<ll>(seat_layout[i].size()); j < j_end; j++) {
            if (seat_layout[i][j] == '.') {
                next_seat_layout[i][j] = '.';
                continue;
            }
            ll adjacent_occupied = std::count_if(std::cbegin(D), std::cend(D), [&](auto const& dd) {
                for (ll c = 1;; c++) {
                    auto const in = i + c * dd.first;
                    auto const jn = j + c * dd.second;
                    if (0 <= in && in < static_cast<ll>(seat_layout.size()) && 0 <= jn
                        && jn < static_cast<ll>(seat_layout[in].size())) {
                        if (seat_layout[in][jn] != '.') {
                            return ((seat_layout[in][jn] == '#')) ? 1 : 0;
                        }
                    } else {
                        return 0;
                    }
                    if (!expanded_view) {
                        return 0;
                    }
                }
            });
            if (seat_layout[i][j] == 'L' && adjacent_occupied == 0) {
                next_seat_layout[i][j] = '#';
            } else if (seat_layout[i][j] == '#' && adjacent_occupied >= adjacent_occupied_min) {
                next_seat_layout[i][j] = 'L';
            } else {
                next_seat_layout[i][j] = seat_layout[i][j];
            }
        }
    }
}

result_t solve1(input_t const& input_data) {
    auto actual_seat_layout = input_data;
    auto next_seat_layout = actual_seat_layout;
    for (;;) {
        evolve(actual_seat_layout, next_seat_layout);
        if (next_seat_layout == actual_seat_layout) {
            break;
        }
        std::swap(next_seat_layout, actual_seat_layout);
    }

    return std::accumulate(std::cbegin(actual_seat_layout),
                           std::cend(actual_seat_layout),
                           0ll,
                           [](auto const& base, auto const& r) {
                               return base + std::count(std::cbegin(r), std::cend(r), '#');
                           });
}

result_t solve2(input_t const& input_data) {
    auto actual_seat_layout = input_data;
    auto next_seat_layout = actual_seat_layout;
    for (;;) {
        evolve(actual_seat_layout, next_seat_layout, true, 5);
        if (next_seat_layout == actual_seat_layout) {
            break;
        }
        std::swap(next_seat_layout, actual_seat_layout);
    }

    return std::accumulate(std::cbegin(actual_seat_layout),
                           std::cend(actual_seat_layout),
                           0ll,
                           [](auto const& base, auto const& r) {
                               return base + std::count(std::cbegin(r), std::cend(r), '#');
                           });
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::cout << elem << ", ";
    // });
    // std::cout << std::endl;
    std::cout << "Read Input Time = "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_input - begin_input).count() << "[ms]"
              << std::endl;

    auto begin_s1 = std::chrono::steady_clock::now();
    auto const s1 = solve1(input_data);
    auto end_s1 = std::chrono::steady_clock::now();
    std::cout << "Solution Part1: " << s1
              << "   -> Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_s1 - begin_s1).count()
              << "[ms]" << std::endl;

    auto begin_s2 = std::chrono::steady_clock::now();
    auto const s2 = solve2(input_data);
    auto end_s2 = std::chrono::steady_clock::now();
    std::cout << "Solution Part2: " << s2
              << "   -> Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_s2 - begin_s2).count()
              << "[ms]" << std::endl;

    return 0;
}