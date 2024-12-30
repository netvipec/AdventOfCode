package main

import "core:fmt"
import "core:os"
import "core:strings"

InputT :: struct {
	towel_patterns:  [dynamic]string,
	desired_designs: [dynamic]string,
}
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	towel_patterns, _, desired_designs := strings.partition(content^, "\n\n")
	for line in strings.split(towel_patterns, ", ") {
		append(&input.towel_patterns, line)
	}
	for line in strings.split_lines_iterator(&desired_designs) {
		append(&input.desired_designs, line)
	}

	return input
}

is_possible :: proc(
	desired_design: string,
	input: InputT,
	DP: ^map[string]int,
) -> int {
	if len(desired_design) == 0 {
		return 1
	}
	elem, found := &DP[desired_design]
	if found {
		return elem^
	}
	// fmt.println("dd:", desired_design)
	possible := 0
	for towel_pattern in input.towel_patterns {
		// fmt.println("tp:", towel_pattern)
		if strings.starts_with(desired_design, towel_pattern) {
			possible += is_possible(
				desired_design[len(towel_pattern):],
				input,
				DP,
			)
		}
	}
	DP[desired_design] = possible
	return possible
}

part1_solve :: proc(input: InputT) -> OutputT {
	DP := make(map[string]int)
	defer delete(DP)

	counter := 0
	for desired_design in input.desired_designs {
		if is_possible(desired_design, input, &DP) > 0 {
			counter += 1
		}
	}

	return fmt.aprintf("%v", counter)
}

part2_solve :: proc(input: InputT) -> OutputT {
	DP := make(map[string]int)
	defer delete(DP)

	counter := 0
	for desired_design in input.desired_designs {
		possibilities := is_possible(desired_design, input, &DP)
		counter += possibilities
	}

	return fmt.aprintf("%v", counter)
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

	input_test_1 := `r, wr, b, g, bwu, rb, gb, br

brwrr
bggr
gbbr
rrbgbr
ubwu
bwurrg
brgr
bbrgwb`

	if !run(&input_test_1, true, true, "6", "16") {
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
