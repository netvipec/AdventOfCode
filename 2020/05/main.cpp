#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::string>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        input_values.push_back(line);
    }
    return input_values;
}

ll get_seat_id(std::string const& boarding_pass) {
    ll power2 = 1 << (boarding_pass.size() - 1);
    ll seat_id = 0;
    for (size_t i = 0; i < boarding_pass.size(); i++, power2 >>= 1) {
        seat_id |= (boarding_pass[i] == 'B') ? power2 : 0;
        seat_id |= (boarding_pass[i] == 'R') ? power2 : 0;
    }

    return seat_id;
}

std::vector<ll> get_boarding_pass(input_t const& input_data) {
    std::vector<ll> boarding_pass;
    std::transform(std::cbegin(input_data),
                   std::cend(input_data),
                   std::back_inserter(boarding_pass),
                   [&](auto const& elem) {
                       return get_seat_id(elem);
                   });
    return boarding_pass;
}

result_t solve1(input_t const& input_data) {
    auto const boarding_pass = get_boarding_pass(input_data);
    auto const m = std::max_element(std::cbegin(boarding_pass), std::cend(boarding_pass));
    return *m;
}

result_t solve2(input_t const& input_data) {
    auto const boarding_pass = get_boarding_pass(input_data);
    assert(boarding_pass.size() > 0);
    auto const mm = std::minmax_element(std::cbegin(boarding_pass), std::cend(boarding_pass));
    ll sum = (*mm.first + *mm.second);
    ll elems = (*mm.second - *mm.first + 1);
    ll seat_sum = sum * elems / 2;
    ll existing_seats_sum = std::accumulate(std::cbegin(boarding_pass), std::cend(boarding_pass), 0);

    return seat_sum - existing_seats_sum;
}

int main() {
    auto const input_data = read_input();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::cout << elem << ",";
    // });
    // std::cout << std::endl;

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}