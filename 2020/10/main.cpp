#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<ll>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        ll a;
        if (!(iss >> a)) {
            break;
        } // error

        input_values.emplace_back(a);
    }
    return input_values;
}

result_t solve1(input_t const& input_data) {
    auto adapters = input_data;
    auto const max_elem = std::max_element(std::cbegin(adapters), std::cend(adapters));
    adapters.push_back(*max_elem + 3);
    std::sort(std::begin(adapters), std::end(adapters));

    ll a3 = 0;
    ll a1 = 1;
    for (unsigned i = 0; i < adapters.size() - 1; ++i) {
        assert((adapters[i + 1] - adapters[i]) == 1 || (adapters[i + 1] - adapters[i]) == 3);
        a1 += (adapters[i + 1] - adapters[i] == 1) ? 1 : 0;
        a3 += (adapters[i + 1] - adapters[i] == 3) ? 1 : 0;
    }
    return a1 * a3;
}

result_t solve2(input_t const& input_data) {
    auto adapters = input_data;
    auto const max_elem = std::max_element(std::cbegin(adapters), std::cend(adapters));
    adapters.push_back(*max_elem + 3);
    adapters.push_back(0);
    std::sort(std::begin(adapters), std::end(adapters));

    std::vector<ll> sol_until(adapters.size());
    sol_until[0] = 1;

    for (ll i = 1; i < static_cast<ll>(adapters.size()); i++) {
        for (ll j = std::max(static_cast<ll>(0), i - 3); j < i; j++) {
            if (adapters[i] - adapters[j] <= 3) {
                sol_until[i] += sol_until[j];
            }
        }
    }
    return sol_until[sol_until.size() - 1];
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