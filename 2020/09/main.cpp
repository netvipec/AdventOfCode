#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<ll>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        std::istringstream iss(line);
        ll n;
        if (!(iss >> n)) {
            break;
        } // error

        input_values.emplace_back(n);
    }
    return input_values;
}

result_t solve1(input_t const& input_data) {
    constexpr ll preamble_count = 25;
    std::unordered_set<ll> preamble;
    preamble.insert(std::cbegin(input_data), std::cbegin(input_data) + preamble_count - 1);

    result_t ans = -1;
    for (ll i = preamble_count, i_end = static_cast<ll>(input_data.size()); i < i_end; i++) {
        bool good = false;
        preamble.insert(input_data[i]);
        for (auto it = std::cbegin(preamble), it_end = std::cend(preamble); it != it_end; it++) {
            if (*it > input_data[i]) {
                continue;
            }
            auto const other_it = preamble.find(input_data[i] - *it);
            if (other_it != std::cend(preamble)) {
                good = true;
                break;
            }
        }
        preamble.erase(input_data[i - preamble_count]);
        if (!good) {
            return input_data[i];
        }
    }
    return ans;
}

result_t solve2(input_t const& input_data) {
    ll const target_sum = solve1(input_data);

    ll b = 0;
    ll e = 0;
    ll sum = 0;
    for (;;) {
        if (sum < target_sum) {
            if (e >= static_cast<ll>(input_data.size())) {
                return -1;
            }
            sum += input_data[e];
            e++;
        } else if (sum > target_sum) {
            sum -= input_data[b];
            b++;
        } else {
            break;
        }
    }

    auto const mm = std::minmax_element(input_data.cbegin() + b, input_data.cbegin() + e);
    return *mm.first + *mm.second;
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