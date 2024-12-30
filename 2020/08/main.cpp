#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::pair<std::string, ll>>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string instruction;
        char sign;
        ll number;
        if (!(iss >> instruction >> sign >> number)) {
            break;
        } // error

        // std::cout << instruction << " " << sign << number << std::endl;
        if (sign == '-') {
            number = -number;
        }
        input_values.emplace_back(instruction, number);
    }
    return input_values;
}

std::pair<ll, bool> run(input_t const& input_data) {
    std::unordered_set<ll> executed_instruction_pointers;
    ll instruction_pointer = 0;
    ll accumulator = 0;
    for (; instruction_pointer < static_cast<int>(input_data.size());) {
        if (!executed_instruction_pointers.insert(instruction_pointer).second) {
            break;
        }

        auto const& instruction = input_data[instruction_pointer];

        if (instruction.first == "acc") {
            accumulator += instruction.second;
        } else if (instruction.first == "jmp") {
            instruction_pointer += instruction.second;
            continue;
        }

        instruction_pointer++;
    }

    return std::make_pair(accumulator, instruction_pointer == static_cast<int>(input_data.size()));
}

result_t solve1(input_t const& input_data) { return run(input_data).first; }

result_t solve2(input_t const& input_data) {
    auto id = input_data;
    for (size_t i = 0; i < input_data.size(); i++) {
        if (input_data[i].first == "jmp" || input_data[i].first == "nop") {
            id[i].first = (input_data[i].first == "jmp") ? "nop" : "jmp";
            auto const res = run(id);
            id[i] = input_data[i];
            if (res.second) {
                return res.first;
            }
        }
    }
    return -1;
}

int main() {
    auto const input_data = read_input();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::cout << elem << ", ";
    // });
    // std::cout << std::endl;

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}