#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<int>;

constexpr ll target_product = 2020;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        ll n;
        if (!(iss >> n)) {
            break;
        } // error

        input_values.push_back(n);
    }
    return input_values;
}

ll solve1(input_t const& input_data) {
    for (size_t i = 0; i < input_data.size() - 2; i++) {
        for (size_t j = i + 1; j < input_data.size() - 1; j++) {
            if (input_data[i] + input_data[j] == target_product) {
                return input_data[i] * input_data[j];
            }
        }
    }
    return -1;
}

ll solve2(input_t const& input_data) {
    for (size_t i = 0; i < input_data.size() - 2; i++) {
        for (size_t j = i + 1; j < input_data.size() - 1; j++) {
            for (size_t k = j + 1; k < input_data.size(); k++) {
                if (input_data[i] + input_data[j] + input_data[k] == 2020) {
                    return input_data[i] * input_data[j] * input_data[k];
                }
            }
        }
    }
    return -1;
}

int main() {
    auto const input_data = read_input();

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}