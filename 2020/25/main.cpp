#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::pair<ll, ll>;
using result1_t = ll;
using result2_t = ll;

static std::vector<std::string> split(std::string const& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

input_t read_input() {
    input_t input_values;

    std::string line1;
    std::getline(std::cin, line1);
    std::string line2;
    std::getline(std::cin, line2);

    input_values.first = std::stoi(line1);
    input_values.second = std::stoi(line2);
    return input_values;
}

ll get_loop_size(ll n) {
    constexpr ll init_value = 7;
    constexpr ll mod = 20201227;

    ll value = init_value;
    ll counter = 1;
    while (value != n) {
        value *= init_value;
        value %= mod;
        counter++;
    }
    return counter;
}

ll modpow(ll base, ll exp) {
    constexpr ll mod = 20201227;

    ll value = 1;
    for (ll i = 0; i < exp; i++) {
        value = value * base;
        value = value % mod;
    }
    return value;
}

result1_t solve1(input_t const& input_data) {
    auto const loop_size1 = get_loop_size(input_data.first);
    auto const loop_size2 = get_loop_size(input_data.second);
    auto const enc1 = modpow(input_data.first, loop_size2);
    auto const enc2 = modpow(input_data.second, loop_size1);
    assert(enc1 == enc2);
    return enc1;
}

result2_t solve2(input_t const& input_data) {
    result2_t ans = -1;
    // TODO: insert code
    return ans;
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