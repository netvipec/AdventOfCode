#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::vector<std::string>>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    std::vector<std::string> group_respose_line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            input_values.push_back(group_respose_line);
            group_respose_line.clear();
            continue;
        }
        group_respose_line.push_back(line);
    }
    input_values.push_back(group_respose_line);
    return input_values;
}

result_t solve1(input_t const& input_data) {
    // Accumulate the quantity of yes response from every group.
    ll ans = std::accumulate(std::cbegin(input_data),
                             std::cend(input_data),
                             0,
                             [](ll base, auto const& group_response_list) {
                                 std::set<char> responses;
                                 // Add yes responses of every person in the group to the set
                                 std::for_each(std::cbegin(group_response_list),
                                               std::cend(group_response_list),
                                               [&](auto const& person_response_list) {
                                                   // Add yes responses of every person to the set
                                                   std::transform(std::cbegin(person_response_list),
                                                                  std::cend(person_response_list),
                                                                  std::inserter(responses, responses.end()),
                                                                  [](auto const& response) {
                                                                      return response;
                                                                  });
                                               });
                                 return base + responses.size();
                             });
    return ans;
}

result_t solve2(input_t const& input_data) {
    // Accumulate the yes responses that every person in the group have.
    ll ans = std::accumulate(
        std::cbegin(input_data),
        std::cend(input_data),
        0,
        [](ll base, auto const& group_response_list) {
            std::map<char, ll> responses;
            // Count yes responses of every person in the group
            std::for_each(std::cbegin(group_response_list),
                          std::cend(group_response_list),
                          [&](auto const& person_response_list) {
                              // Count yes responses of every person
                              std::for_each(std::cbegin(person_response_list),
                                            std::cend(person_response_list),
                                            [&](auto const& response) {
                                                responses[response]++;
                                            });
                          });
            return base + std::count_if(std::cbegin(responses), std::cend(responses), [&](auto const& elem) {
                       return elem.second == static_cast<ll>(group_response_list.size());
                   });
        });
    return ans;
}

int main() {
    auto const input_data = read_input();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::for_each(std::cbegin(elem), std::cend(elem), [](auto const& e) {
    //         std::cout << e << ", ";
    //     });
    //     std::cout << std::endl;
    // });
    // std::cout << std::endl;

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}
