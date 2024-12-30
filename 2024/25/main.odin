package main

import "core:fmt"
import "core:os"
import "core:strings"

InputT :: struct {
	locks: [dynamic][dynamic]int,
	keys:  [dynamic][dynamic]int,
	size:  int,
}
OutputT :: string

convert_grid :: proc(
	grid: ^[dynamic][dynamic]rune,
	is_lock: bool,
) -> [dynamic]int {
	ROW := len(grid)
	COL := len(grid[0])

	heights := [dynamic]int{}
	for j in 0 ..< COL {
		height := -1
		for i in 0 ..< ROW {
			if is_lock {
				if grid[i][j] == '.' {
					height = i - 1
					break
				}
			} else {
				if grid[ROW - i - 1][j] == '.' {
					height = i - 1
					break
				}
			}
		}
		append(&heights, height)
	}
	return heights
}

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	locks_keys := strings.split(content^, "\n\n")
	for &lock_key in locks_keys {
		// fmt.println(lock_key)
		grid := [dynamic][dynamic]rune{}
		for line in strings.split_lines_iterator(&lock_key) {
			row := [dynamic]rune{}
			for cell in line {
				append(&row, cell)
			}
			append(&grid, row)
		}
		// fmt.println(grid)

		input.size = len(grid) - 2

		if grid[0][0] == '#' {
			append(&input.locks, convert_grid(&grid, true))
		} else {
			append(&input.keys, convert_grid(&grid, false))
		}
	}

	return input
}

fit :: proc(key, lock: ^[dynamic]int, size: int) -> bool {
	assert(len(key) == len(lock))
	fmt.println(lock, key)
	for i in 0 ..< len(key) {
		if key[i] + lock[i] > size {
			return false
		}
	}
	return true
}

part1_solve :: proc(input: ^InputT) -> OutputT {
	acc := 0
	for j in 0 ..< len(input.locks) {
		for i in 0 ..< len(input.keys) {
			if fit(&input.keys[i], &input.locks[j], input.size) {
				fmt.println(
					"Fit. Lock:",
					input.locks[j],
					", Key:",
					input.keys[i],
				)
				acc += 1
			}
		}
	}
	return fmt.aprintf("%v", acc)
}

part2_solve :: proc(input: ^InputT) -> OutputT {
	return fmt.aprintf("%v", 0)
}

run :: proc(
	content: ^string,
	part1, part2: bool,
	expected_part1, expected_part2: string,
) -> bool {
	input := read_input(content)
	// fmt.println(input)

	if part1 {
		part1_solution := part1_solve(&input)
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
		part2_solution := part2_solve(&input)
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

	input_test_1 := `#####
.####
.####
.####
.#.#.
.#...
.....

#####
##.##
.#.##
...##
...#.
...#.
.....

.....
#....
#....
#...#
#.#.#
#.###
#####

.....
.....
#.#..
###..
###.#
###.#
#####

.....
.....
.....
#....
#.#..
#.#.#
#####`

	if !run(&input_test_1, true, false, "3", "-1") {
		bad += 1
	}

	return bad
}

main :: proc() {
	fail_tests := run_tests()
	if fail_tests != 0 {
		fmt.println("Some tests fail:", fail_tests)
		return
	} else {
		fmt.println("All tests pass.")
	}

	data := os.read_entire_file("input") or_else os.exit(1)
	defer delete(data)
	s := string(data)

	input := read_input(&s)
	// fmt.println(input)

	fmt.println("Executing Real Part1...")
	part1 := part1_solve(&input)
	fmt.println("Part1:", part1)
	fmt.println("Executing Real Part2...")
	part2 := part2_solve(&input)
	fmt.println("Part2:", part2)
}
