package main

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"

InputT :: struct {
	size:          [2]int,
	falling_bytes: [dynamic][2]int,
	falling_size:  int,
}
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	for line in strings.split_lines_iterator(content) {
		y, _, x := strings.partition(line, ",")
		append(&input.falling_bytes, [2]int{strconv.atoi(x), strconv.atoi(y)})
	}

	return input
}

DIR := [?][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}} // up right down left

Cell :: struct {
	pos:       [2]int,
	path_size: int,
}

part1_solve_helper :: proc(
	input: InputT,
	corrupted: map[[2]int]struct {},
) -> int {
	process := [dynamic]Cell{}
	seen := make(map[[2]int]struct {})
	defer delete(seen)

	start_pos := [2]int{0, 0}
	end_pos := [2]int{input.size.x - 1, input.size.y - 1}
	append(&process, Cell{start_pos, 0})
	seen[start_pos] = {}
	for len(process) > 0 {
		current := pop_front(&process)
		if current.pos == end_pos {
			return current.path_size
		}

		for d in DIR {
			new_pos := current.pos + d
			if new_pos.x < 0 ||
			   new_pos.x >= input.size.x ||
			   new_pos.y < 0 ||
			   new_pos.y >= input.size.y {
				continue
			}

			if new_pos in corrupted {
				continue
			}

			if new_pos in seen {
				continue
			}

			seen[new_pos] = {}
			append(&process, Cell{new_pos, current.path_size + 1})
		}
	}
	return 0
}

get_corrupted :: proc(
	input: InputT,
	falling_size: int,
) -> map[[2]int]struct {} {
	corrupted := make(map[[2]int]struct {})
	for i in 0 ..< falling_size {
		corrupted[input.falling_bytes[i]] = {}
	}
	return corrupted
}

print_grid :: proc(corrupted: map[[2]int]struct {}, size: [2]int) {
	for x in 0 ..< size.x {
		for y in 0 ..< size.y {
			cell := [2]int{x, y}
			if cell in corrupted {
				fmt.print("#")
			} else {
				fmt.print(".")
			}
		}
		fmt.println()
	}
	fmt.println()
	fmt.println()
}

part1_solve :: proc(input: InputT) -> OutputT {
	corrupted := get_corrupted(input, input.falling_size)
	defer delete(corrupted)

	result := part1_solve_helper(input, corrupted)
	return fmt.aprintf("%v", result)
}

part2_solve :: proc(input: InputT) -> OutputT {
	for i in input.falling_size + 1 ..< len(input.falling_bytes) {
		corrupted := get_corrupted(input, i)
		result := part1_solve_helper(input, corrupted)
		// print_grid(corrupted, input.size)
		if result == 0 {
			return fmt.aprintf(
				"%v,%v",
				input.falling_bytes[i - 1].y,
				input.falling_bytes[i - 1].x,
			)
		}
	}
	return fmt.aprintf("%v", 0)
}

run :: proc(
	content: ^string,
	part1, part2: bool,
	expected_part1, expected_part2: string,
) -> bool {
	input := read_input(content)
	// fmt.println(input)
	input.size = [2]int{7, 7}
	input.falling_size = 12

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

	input_test_1 := `5,4
4,2
4,5
3,0
2,1
6,3
2,4
1,5
0,6
3,3
2,6
5,1
1,2
5,5
2,5
6,5
1,4
0,4
6,4
1,1
6,1
1,0
0,5
1,6
2,0`

	if !run(&input_test_1, true, true, "22", "6,1") {
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
	input.size = [2]int{71, 71}
	input.falling_size = 1024
	// fmt.println(input)

	part1 := part1_solve(input)
	fmt.println("Part1:", part1)
	part2 := part2_solve(input)
	fmt.println("Part2:", part2)
}
