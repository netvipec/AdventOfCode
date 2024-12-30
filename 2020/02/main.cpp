#include <bits/stdc++.h>

struct d {
    int l_min;
    int l_max;
    char letter;
};

using ll = int64_t;
using input_t = std::vector<std::pair<d, std::string>>;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int a;
        int b;
        char c;
        char junk;
        std::string str;
        if (!(iss >> a >> junk >> b >> c >> junk >> str)) {
            break;
        } // error

        // std::cout << a << "-" << b << " " << c << ": " << str << std::endl;
        input_values.push_back(std::make_pair(d{ a, b, c }, str));
    }
    return input_values;
}

ll solve1(input_t const& input_data) {
    return std::count_if(std::cbegin(input_data), std::cend(input_data), [](auto const& p) {
        auto const lc = std::count(p.second.cbegin(), p.second.cend(), p.first.letter);
        return p.first.l_min <= lc && lc <= p.first.l_max;
    });
}

ll solve2(input_t const& input_data) {
    return std::count_if(std::cbegin(input_data), std::cend(input_data), [](auto const& p) {
        return (p.second[p.first.l_min - 1] == p.first.letter) ^ (p.second[p.first.l_max - 1] == p.first.letter);
    });
}

int main() {
    auto const input_data = read_input();
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){ std::cout << elem << ",
    // "; }); std::cout << std::endl;

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}