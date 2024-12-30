#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<ll>;
using result_t = ll;

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

    std::string line;
    std::getline(std::cin, line);
    auto const input_values_str = split(line, ',');

    for (auto const& ivs : input_values_str) {
        input_values.emplace_back(std::stoi(ivs));
    }
    return input_values;
}

result_t solve1(input_t const& input_data) {
    std::vector<ll> last_seen;
    for (size_t i = 0; i < input_data.size(); i++) {
        last_seen.emplace_back(input_data[i]);
        // std::cout << i << " = " << input_data[i] << std::endl;
    }
    auto it = last_seen.cend();
    for (size_t i = input_data.size(); i < 2020; i++) {
        if (it == last_seen.cend()) {
            last_seen.emplace_back(0);
            it = std::find(std::crbegin(last_seen) + 1, std::crend(last_seen), 0).base();
        } else {
            auto const it2 = std::find(std::crbegin(last_seen) + 1, std::crend(last_seen), last_seen.back()).base();
            auto const nn = last_seen.size() - std::distance(std::cbegin(last_seen), it2);
            last_seen.emplace_back(nn);
            auto const it3 = std::find(std::crbegin(last_seen) + 1, std::crend(last_seen), nn);
            if (it3 == last_seen.crend()) {
                it = last_seen.cend();
            } else {
                it = it3.base();
            }
        }
        // std::cout << i << " = " << last_seen.back() << std::endl;
    }
    return last_seen.back();
}

result_t solve2(input_t const& input_data) {
    std::unordered_map<ll, std::pair<ll, ll>> last_seen;
    for (size_t i = 0; i < input_data.size(); i++) {
        last_seen.emplace(input_data[i], std::make_pair(0, i + 1));
        // std::cout << i << " = " << input_data[i] << std::endl;
    }
    auto it = last_seen.end();
    ll last = -1;
    for (size_t i = input_data.size(); i < 30000000; i++) {
        if (it == last_seen.end()) {
            it = last_seen.find(0);
            std::swap(it->second.first, it->second.second);
            it->second.second = i + 1;
            last = 0;
        } else {
            auto const nn = it->second.second - it->second.first;
            auto const it2 = last_seen.find(nn);
            if (it2 == last_seen.end()) {
                last_seen[nn] = std::make_pair(0, i + 1);
                it = last_seen.end();
            } else {
                last_seen[nn] = std::make_pair(it2->second.second, i + 1);
                it = it2;
            }
            last = nn;
        }
        // std::cout << i << " = " << last << std::endl;
    }
    return last;
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