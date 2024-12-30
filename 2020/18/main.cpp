#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::string>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        input_values.emplace_back(line);
    }
    return input_values;
}

template <int PriorityAdd, int PriorityMul>
result_t evaluate(std::string const& expression, size_t& index, ll priority = 0) {
    ll idx = -1;
    result_t n = -1;
    for (size_t i = index; i < expression.size(); i++) {
        if ('0' <= expression[i] && expression[0] <= '9') {
            idx = i;
            continue;
        } else if (expression[i] == ' ' || expression[i] == ')') {
            if (idx != -1) {
                n = std::stoi(expression.substr(idx, i - idx));
                idx = -1;
            }
            if (expression[i] == ')') {
                if (priority > 0) {
                    index = i - 1;
                } else {
                    index = i;
                }
                return n;
            }
            continue;
        }
        switch (expression[i]) {
            case ' ':
                continue;
            case '*':
                assert(n != -1);
                if (priority >= PriorityMul) {
                    index = i - 1;
                    return n;
                }
                i += 1;
                n *= evaluate<PriorityAdd, PriorityMul>(expression, i, PriorityMul);
                break;
            case '+':
                assert(n != -1);
                if (priority >= PriorityAdd) {
                    index = i - 1;
                    return n;
                }
                i += 1;
                n += evaluate<PriorityAdd, PriorityMul>(expression, i, PriorityAdd);
                break;
            case '(':
                assert(n == -1);
                i += 1;
                n = evaluate<PriorityAdd, PriorityMul>(expression, i, 0);
                assert(expression.size() == i || expression[i] == ')');
                break;
            default:
                assert(false);
                break;
        }
    }
    index = expression.size();
    if (idx != -1) {
        n = std::stoi(expression.substr(idx));
    }
    return n;
}

result_t solve1(input_t const& input_data) {
    result_t ans = 0;
    for (auto const& expression : input_data) {
        size_t index = 0;
        ans += evaluate<1, 1>(expression, index);
    }
    return ans;
}

result_t solve2(input_t const& input_data) {
    result_t ans = 0;
    for (auto const& expression : input_data) {
        size_t index = 0;
        ans += evaluate<2, 1>(expression, index);
    }
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