package main

import "core:fmt"
import "core:math"
import "core:os"
import "core:strconv"
import "core:strings"

TEST :: false

InputT :: [dynamic]int
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	for line in strings.split_lines_iterator(content) {
		stones := strings.split(line, " ")
		for stone in stones {
			append(&input, strconv.atoi(stone))
		}
	}

	return input
}

expand :: proc(input: InputT) -> InputT {
	new_stones := InputT{}
	for i in 0 ..< len(input) {
		if input[i] == 0 {
			append(&new_stones, 1)
		} else {
			digits := math.count_digits_of_base(input[i], 10)
			if digits % 2 == 0 {
				mask := cast(int)math.pow(10.0, cast(f32)(digits / 2))
				append(&new_stones, input[i] / mask)
				append(&new_stones, input[i] % mask)
			} else {
				append(&new_stones, input[i] * 2024)
			}
		}
	}
	return new_stones
}

part1_solve :: proc(input_original: InputT) -> OutputT {
	input := InputT{}
	resize(&input, len(input_original))
	copy(input[:], input_original[:])

	for _ in 0 ..< 25 {
		new_stones := expand(input)

		resize(&input, len(new_stones))
		copy(input[:], new_stones[:])
	}
	return fmt.aprintf("%v", len(input))
}

get_expansion_size :: proc(
	stone: int,
	cache: ^map[[2]int]int,
	depth: int,
) -> int {
	if depth == 0 {
		return 1
	}

	key := [2]int{stone, depth}
	if key in cache {
		return cache[key]
	}

	expand_stones := expand(InputT{stone})
	value := 0
	for expand_stone in expand_stones {
		value += get_expansion_size(expand_stone, cache, depth - 1)
	}
	cache^[key] = value
	return value
}

part2_solve :: proc(input: InputT) -> OutputT {
	cache := make(map[[2]int]int)

	dp := 0
	for stone in input {
		dp += get_expansion_size(stone, &cache, 75)
	}
	return fmt.aprintf("%v", dp)
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
