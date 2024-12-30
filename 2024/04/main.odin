package main

import "core:fmt"
import "core:os"
import "core:strings"

TEST :: false

InputT :: [dynamic]string
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}
	for line in strings.split_lines_iterator(content) {
		append(&input, line)
	}

	return input
}

search_orderings :: proc(
	input: InputT,
	xmas_ordering: [dynamic][dynamic]string,
) -> OutputT {
	xmas := 0
	ROW := len(input)
	COL := len(input[0])
	for x in 0 ..< ROW {
		for y in 0 ..< COL {
			for candidate in xmas_ordering {
				if x + len(candidate) > ROW {
					continue
				}
				match := true
				candidate_loop: for crow, cx in candidate {
					if y + len(crow) > COL {
						match = false
						break
					}
					for cell, cy in crow {
						if cell != '.' &&
						   cell != cast(rune)input[x + cx][y + cy] {
							match = false
							break candidate_loop
						}
					}
				}
				if match {
					xmas += 1
					// fmt.println(x, y, candidate)
				}
			}
		}
	}

	return fmt.aprintf("%v", xmas)
}

part1_solve :: proc(input: InputT) -> OutputT {
	xmas_ordering := [dynamic][dynamic]string {
		{"XMAS"},
		{"SAMX"},
		{"X", "M", "A", "S"},
		{"S", "A", "M", "X"},
		{"X", ".M", "..A", "...S"},
		{"...X", "..M", ".A", "S"},
		{"S", ".A", "..M", "...X"},
		{"...S", "..A", ".M", "X"},
	}

	return search_orderings(input, xmas_ordering)
}

part2_solve :: proc(input: InputT) -> OutputT {
	xmas_ordering := [dynamic][dynamic]string {
		{"M.M", ".A.", "S.S"},
		{"S.S", ".A.", "M.M"},
		{"M.S", ".A.", "M.S"},
		{"S.M", ".A.", "S.M"},
	}

	return search_orderings(input, xmas_ordering)
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
