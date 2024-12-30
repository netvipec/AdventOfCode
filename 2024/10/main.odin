package main

import "core:fmt"
import "core:os"
import "core:strings"

TEST :: false

InputT :: [dynamic][dynamic]int
OutputT :: string

Trail :: struct {
	id:  int,
	pos: [2]int,
}

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	for line in strings.split_lines_iterator(content) {
		row := [dynamic]int{}
		for cell in line {
			append(&row, cast(int)(cell - '0'))
		}
		append(&input, row)
	}

	return input
}

part1_solve :: proc(input: InputT) -> OutputT {
	R := len(input)
	C := len(input[0])

	process := [dynamic]Trail{}
	seen := make(map[Trail]struct {})
	endpoints := make(map[int]int)

	DIR := [?][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}

	trail_id := 1
	for r in 0 ..< R {
		for c in 0 ..< C {
			if input[r][c] == 0 {
				append(&process, Trail{trail_id, [2]int{r, c}})
				trail_id += 1
			}
		}
	}

	for len(process) > 0 {
		current := pop(&process)

		for d in DIR {
			new_pos := current.pos + d
			if new_pos.x < 0 ||
			   new_pos.x >= R ||
			   new_pos.y < 0 ||
			   new_pos.y >= C {
				continue
			}

			if input[new_pos.x][new_pos.y] !=
			   input[current.pos.x][current.pos.y] + 1 {
				continue
			}

			new_trail := Trail{current.id, new_pos}
			if new_trail in seen {
				continue
			}
			seen[new_trail] = {}

			if input[new_trail.pos.x][new_trail.pos.y] == 9 {
				endpoints[new_trail.id] += 1
			}

			append(&process, new_trail)
		}
	}

	acc := 0
	for _, paths in endpoints {
		acc += paths
	}

	return fmt.aprintf("%v", acc)
}

part2_solve :: proc(input: InputT) -> OutputT {
	R := len(input)
	C := len(input[0])

	process := [dynamic]Trail{}
	endpoints := make(map[int]int)

	DIR := [?][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}

	trail_id := 1
	for r in 0 ..< R {
		for c in 0 ..< C {
			if input[r][c] == 0 {
				append(&process, Trail{trail_id, [2]int{r, c}})
				trail_id += 1
			}
		}
	}

	for len(process) > 0 {
		current := pop(&process)

		for d in DIR {
			new_pos := current.pos + d
			if new_pos.x < 0 ||
			   new_pos.x >= R ||
			   new_pos.y < 0 ||
			   new_pos.y >= C {
				continue
			}

			if input[new_pos.x][new_pos.y] !=
			   input[current.pos.x][current.pos.y] + 1 {
				continue
			}

			new_trail := Trail{current.id, new_pos}

			if input[new_trail.pos.x][new_trail.pos.y] == 9 {
				endpoints[new_trail.id] += 1
			}

			append(&process, new_trail)
		}
	}

	acc := 0
	for _, paths in endpoints {
		acc += paths
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
