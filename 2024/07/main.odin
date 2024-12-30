package main

import "core:fmt"
import "core:math"
import "core:os"
import "core:strconv"
import "core:strings"

TEST :: false

Equation :: struct {
	result:   int,
	operands: [dynamic]int,
}

InputT :: [dynamic]Equation
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	for line in strings.split_lines_iterator(content) {
		result, _, operands := strings.partition(line, ": ")
		operands_list := strings.split(operands, " ")
		result_value := strconv.atoi(result)
		operands_value := [dynamic]int{}
		for operand in operands_list {
			append(&operands_value, strconv.atoi(operand))
		}
		append(&input, Equation{result_value, operands_value})
	}

	return input
}

get_concat :: proc(l, r: int) -> int {
	digits := math.count_digits_of_base(r, 10)
	result := l
	for _ in 0 ..< digits {
		result *= 10
	}
	return result + r
}

possible :: proc(
	equation: Equation,
	result: int,
	is_part2: bool = false,
	index: int = 1,
) -> bool {
	if result > equation.result {
		return false
	}
	if index == len(equation.operands) {
		return result == equation.result
	}

	if possible(
		equation,
		result * equation.operands[index],
		is_part2,
		index + 1,
	) {
		return true
	}
	if is_part2 &&
	   possible(
		   equation,
		   get_concat(result, equation.operands[index]),
		   is_part2,
		   index + 1,
	   ) {
		return true
	}
	return possible(
		equation,
		result + equation.operands[index],
		is_part2,
		index + 1,
	)
}

part1_solve :: proc(input: InputT) -> OutputT {
	acc := 0
	for equation in input {
		if possible(equation, equation.operands[0]) {
			acc += equation.result
		}
	}
	return fmt.aprintf("%v", acc)
}

part2_solve :: proc(input: InputT) -> OutputT {
	acc := 0
	for equation in input {
		if possible(equation, equation.operands[0], true) {
			acc += equation.result
		}
	}
	return fmt.aprintf("%v", acc)
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
