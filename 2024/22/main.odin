package main

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"

InputT :: [dynamic]int
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	for line in strings.split_lines_iterator(content) {
		append(&input, strconv.atoi(line))
	}

	return input
}

hash :: proc(secret: int) -> int {
	module :: 16777216
	new_secret := secret
	new_secret ~= (secret * 64)
	new_secret %= module
	new_secret ~= (new_secret / 32)
	new_secret %= module
	new_secret ~= (new_secret * 2048)
	new_secret %= module
	return new_secret
}

part1_solve :: proc(input: InputT) -> OutputT {
	secrets := [dynamic]int{}
	for secret in input {
		s := secret
		for _ in 0 ..< 2000 {
			s = hash(s)
		}
		append(&secrets, s)
	}

	acc := 0
	for secret in secrets {
		acc += secret
	}
	// fmt.println(secrets)
	return fmt.aprintf("%v", acc)
}

part2_solve :: proc(input: InputT) -> OutputT {
	bananas := make(map[[4]int]int)
	defer delete(bananas)

	changes := [dynamic][dynamic][2]int{}
	for secret in input {
		append(&changes, [dynamic][2]int{})
		seen := make(map[[4]int]struct {})
		defer delete(seen)

		s := secret
		for i in 0 ..< 2000 {
			new_s := hash(s)
			append(
				&changes[len(changes) - 1],
				[2]int{(new_s % 10) - (s % 10), new_s % 10},
			)
			s = new_s
			if len(changes[len(changes) - 1]) >= 4 {
				key := [4]int {
					changes[len(changes) - 1][i - 3][0],
					changes[len(changes) - 1][i - 2][0],
					changes[len(changes) - 1][i - 1][0],
					changes[len(changes) - 1][i][0],
				}
				if key in seen {
					continue
				}
				seen[key] = {}
				bananas[key] += new_s % 10
			}
		}
	}

	max_bananas := 0
	for _, bananas in bananas {
		max_bananas = max(max_bananas, bananas)
	}

	return fmt.aprintf("%v", max_bananas)
}

run :: proc(
	content: ^string,
	part1, part2: bool,
	expected_part1, expected_part2: string,
) -> bool {
	input := read_input(content)
	// fmt.println(input)

	if part1 {
		part1_solution := part1_solve(input)
		if part1_solution != expected_part1 {
			fmt.println(
				"--------------- Part1 - Expected fail: (calculated)",
				part1_solution,
				"!=",
				expected_part1,
				"(expected)",
			)
			return false
		}
	}
	if part2 {
		part2_solution := part2_solve(input)
		if part2_solution != expected_part2 {
			fmt.println(
				"--------------- Part2 - Expected fail: (calculated)",
				part2_solution,
				"!=",
				expected_part2,
				"(expected)",
			)
			return false
		}
	}
	return true
}

run_tests :: proc() -> int {
	bad := 0

	input_test_2 := InputT {
		123,
		15887950,
		16495136,
		527345,
		704524,
		1553684,
		12683156,
		11100544,
		12249484,
		7753432,
		5908254,
	}
	for i in 0 ..< len(input_test_2) - 1 {
		hash_value := hash(input_test_2[i])
		if hash_value != input_test_2[i + 1] {
			fmt.println("wrong hash", i + 1, hash_value, input_test_2[i + 1])
			bad += 1
			break
		}
	}

	// part2_solve(input_test_2)

	input_test_1 := `1
10
100
2024`

	if !run(&input_test_1, true, false, "37327623", "-1") {
		bad += 1
	}

	input_test_3 := `1
2
3
2024`

	if !run(&input_test_3, false, true, "-1", "23") {
		bad += 1
	}

	return bad
}

main :: proc() {
	fail_tests := run_tests()
	if fail_tests != 0 {
		fmt.println("Some tests fail:", fail_tests)
		return
	}

	data := os.read_entire_file("input") or_else os.exit(1)
	defer delete(data)
	s := string(data)

	input := read_input(&s)
	// fmt.println(input)

	part1 := part1_solve(input)
	fmt.println("Part1:", part1)
	part2 := part2_solve(input)
	fmt.println("Part2:", part2)
}
