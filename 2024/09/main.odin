package main

import "core:fmt"
import "core:os"

TEST :: false

File :: struct {
	offset: int,
	size:   int,
	id:     int,
}
InputT :: [dynamic]File
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	offset := 0
	for i in 0 ..< len(content) {
		block_size := int(content^[i] - '0')
		if i % 2 == 0 {
			append(&input, File{offset, block_size, i / 2})
		}
		offset += block_size
	}

	return input
}

find_first_gap :: proc(input: InputT) -> int {
	offset := 0
	for i in 0 ..< len(input) {
		if input[i].offset - offset > 0 {
			return i
		}
		offset = input[i].offset + input[i].size
	}
	return -1
}

get_free_space :: proc(input: InputT, index: int) -> int {
	return(
		input[index].offset -
		(input[index - 1].offset + input[index - 1].size) \
	)
}

get_free_space_offset :: proc(input: InputT, index: int) -> int {
	return input[index - 1].offset + input[index - 1].size
}

checksum :: proc(input: InputT) -> int {
	checksum_res := 0
	for file in input {
		for i in 0 ..< file.size {
			checksum_res += (file.offset + i) * file.id
		}
	}
	return checksum_res
}

find_first_gap_2 :: proc(input: InputT, size: int) -> int {
	offset := 0
	for i in 0 ..< len(input) {
		if input[i].offset - offset >= size {
			return i
		}
		offset = input[i].offset + input[i].size
	}
	return -1
}

part1_solve :: proc(input_original: InputT) -> OutputT {
	input := [dynamic]File{}
	resize(&input, len(input_original))
	copy(input[:], input_original[:])

	back_index := len(input) - 1
	for {
		if input[back_index].size == 0 {
			ordered_remove(&input, back_index)
			back_index -= 1
			continue
		}

		first_gap := find_first_gap(input)

		if first_gap < 0 {
			break
		}

		free_space := get_free_space(input, first_gap)
		move_space := min(free_space, input[back_index].size)
		inject_at(
			&input,
			first_gap,
			File {
				get_free_space_offset(input, first_gap),
				move_space,
				input[back_index].id,
			},
		)
		back_index += 1
		input[back_index].size -= move_space
	}
	return fmt.aprintf("%v", checksum(input))
}

part2_solve :: proc(input: InputT) -> OutputT {
	input := input
	back_index := len(input) - 1
	for {
		if back_index == 0 {
			break
		}
		if input[back_index].size == 0 {
			ordered_remove(&input, back_index)
			back_index -= 1
			continue
		}

		first_gap := find_first_gap_2(input, input[back_index].size)
		if first_gap < 0 || first_gap > back_index {
			back_index -= 1
			continue
		}

		free_space := get_free_space(input, first_gap)
		move_space := min(free_space, input[back_index].size)
		inject_at(
			&input,
			first_gap,
			File {
				get_free_space_offset(input, first_gap),
				move_space,
				input[back_index].id,
			},
		)
		back_index += 1
		input[back_index].size -= move_space
	}
	return fmt.aprintf("%v", checksum(input))
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
