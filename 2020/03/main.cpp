#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::string>;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty())
            break;
        input_values.push_back(line);
    }
    return input_values;
}

ll trees_slope(ll x, ll y, input_t const& input_data) {
    auto const large = input_data.front().size();
    assert(input_data.front().size() == input_data.back().size());

    ll trees = 0;
    for (ll xx = x, yy = y; yy < static_cast<ll>(input_data.size()); yy += y, xx = (xx + x) % large) {
        if (input_data[yy][xx] == '#') {
            trees++;
        }
    }
    return trees;
}

ll solve1(input_t const& input_data) {
    ll x = 3;
    ll y = 1;

    return trees_slope(x, y, input_data);
}

ll solve2(input_t const& input_data) {
    std::vector<std::pair<ll, ll>> slopes{ { 1, 1 }, { 3, 1 }, { 5, 1 }, { 7, 1 }, { 1, 2 } };

    return std::accumulate(std::cbegin(slopes), std::cend(slopes), 1LL, [&](auto base, auto const& slope) {
        return base * trees_slope(slope.first, slope.second, input_data);
    });
}

int main() {
    auto const input_data = read_input();
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){ std::cout << elem <<
    // ",
    // "; }); std::cout << std::endl;

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}