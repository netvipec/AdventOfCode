package main

import "core:fmt"
import "core:os"
import "core:slice"
import "core:strconv"
import "core:strings"

TEST :: false

InputT :: [dynamic][dynamic]int
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := make([dynamic][dynamic]int)
	for line in strings.split_lines_iterator(content) {
		list := strings.split(line, " ")
		l := [dynamic]int{}
		for n in list {
			append(&l, strconv.atoi(n))
		}
		append(&input, l)
	}

	return input
}

is_safe :: proc(diff: [dynamic]int) -> bool {
	if abs(diff[0]) < 1 || abs(diff[0]) > 3 {
		return false
	}
	for i in 1 ..< len(diff) {
		if diff[i] * diff[i - 1] < 0 {
			return false
		}
		if abs(diff[i]) < 1 || abs(diff[i]) > 3 {
			return false
		}
	}
	return true
}

is_safe_line :: proc(l: [dynamic]int) -> bool {
	d := [dynamic]int{}
	for i in 1 ..< len(l) {
		append(&d, l[i] - l[i - 1])
	}

	return is_safe(d)
}

part1_solve :: proc(input: InputT) -> OutputT {
	safe := slice.count_proc(
		input[:],
		proc(line: [dynamic]int) -> bool {return is_safe_line(line)},
	)
	return fmt.aprintf("%v", safe)
}

is_safe_line2 :: proc(l: [dynamic]int, indexes: [dynamic]int) -> bool {
	d := [dynamic]int{}
	for i in 1 ..< len(indexes) {
		append(&d, l[indexes[i]] - l[indexes[i - 1]])
	}

	return is_safe(d)
}

part2_solve :: proc(input: InputT) -> OutputT {
	safe := slice.count_proc(input[:], proc(line: [dynamic]int) -> bool {
		if is_safe_line(line) {
			return true
		}
		for i in 0 ..< len(line) {
			indexes := [dynamic]int{}
			for j in 0 ..< len(line) {
				if i == j {
					continue
				}
				append(&indexes, j)
			}
			if is_safe_line2(line, indexes) {
				return true
			}
		}
		return false
	})
	return fmt.aprintf("%v", safe)
}

main :: proc() {
	data :=
		os.read_entire_file("input.test" if TEST else "input") or_else os.exit(
			1,
		)
	defer delete(data)
	s := string(data)

	input := read_input(&s)
	// fmt.println(input)

	part1 := part1_solve(input)
	fmt.println("Part1:", part1)
	part2 := part2_solve(input)
	fmt.println("Part2:", part2)
}
