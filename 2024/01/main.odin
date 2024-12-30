package main

import "core:fmt"
import "core:os"
import "core:slice"
import "core:strconv"
import "core:strings"

TEST :: false

InputT :: [2][dynamic]int
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}
	for line in strings.split_lines_iterator(content) {
		f, _, l := strings.partition(line, "   ")
		fn := strconv.atoi(f)
		ln := strconv.atoi(l)
		append(&input[0], fn)
		append(&input[1], ln)
	}

	return input
}

part1_solve :: proc(input: InputT) -> OutputT {
	input := input
	slice.sort(input[0][:])
	slice.sort(input[1][:])

	acc := 0
	for i in 0 ..< len(input[0]) {
		acc += abs(input[1][i] - input[0][i])
	}

	return fmt.aprintf("%v", acc)
}

get_counters :: proc(numbers: [dynamic]int) -> map[int]int {
	m := make(map[int]int)
	for n in numbers {
		m[n] += 1
	}
	return m
}

part2_solve :: proc(input: InputT) -> OutputT {
	sm := get_counters(input[1])

	sim := 0
	for i in 0 ..< len(input[0]) {
		sim += input[0][i] * sm[input[0][i]]
	}
	return fmt.aprintf("%v", sim)
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
