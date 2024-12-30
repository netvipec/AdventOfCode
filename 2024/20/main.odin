package main

import "core:fmt"
import "core:os"
import "core:strings"

InputT :: struct {
	grid:       [dynamic][dynamic]rune,
	start:      [2]int,
	end:        [2]int,
	min_cheats: int,
}
OutputT :: string

BfsData :: struct {
	pos:  [2]int,
	cost: int,
}

DIR := [?][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}} // up right down left

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	x := 0
	for line in strings.split_lines_iterator(content) {
		row := [dynamic]rune{}
		for cell, y in line {
			c := cell
			if c == 'S' {
				input.start = [2]int{x, y}
				c = '.'
			}
			if c == 'E' {
				input.end = [2]int{x, y}
				c = '.'
			}
			append(&row, c)
		}
		append(&input.grid, row)
		x += 1
	}

	return input
}

bfs :: proc(input: InputT, start: [2]int, cache: ^map[[2]int]int) {
	ROW := len(input.grid)
	COL := len(input.grid[0])

	process := [dynamic]BfsData{}
	seen := make(map[[2]int]struct {})
	defer delete(seen)

	append(&process, BfsData{start, 0})
	seen[start] = {}
	for len(process) > 0 {
		current := pop_front(&process)
		cache[current.pos] = current.cost
		for d in DIR {
			new_pos := current.pos + d
			if new_pos.x < 0 ||
			   new_pos.x >= ROW ||
			   new_pos.y < 0 ||
			   new_pos.y >= COL {
				continue
			}
			if input.grid[new_pos.x][new_pos.y] == '.' {
				if new_pos in seen {
					continue
				}
				seen[new_pos] = {}
				append(&process, BfsData{new_pos, current.cost + 1})
			}
		}
	}
}

part1_solve :: proc(input: InputT) -> OutputT {
	path_from_origin := make(map[[2]int]int)
	path_to_destination := make(map[[2]int]int)
	bfs(input, input.start, &path_from_origin)
	bfs(input, input.end, &path_to_destination)

	baseline := path_from_origin[input.end]
	acc := 0
	for o_pos, o_cost in path_from_origin {
		for d_pos, d_cost in path_to_destination {
			diff_x := abs(o_pos.x - d_pos.x)
			diff_y := abs(o_pos.y - d_pos.y)
			if diff_x + diff_y <= 2 &&
			   o_cost + d_cost + diff_x + diff_y <=
				   baseline - input.min_cheats {
				acc += 1
			}
		}
	}
	return fmt.aprintf("%v", acc)
}

part2_solve :: proc(input: InputT) -> OutputT {
	path_from_origin := make(map[[2]int]int)
	path_to_destination := make(map[[2]int]int)
	bfs(input, input.start, &path_from_origin)
	bfs(input, input.end, &path_to_destination)

	baseline := path_from_origin[input.end]
	acc := 0
	for o_pos, o_cost in path_from_origin {
		for d_pos, d_cost in path_to_destination {
			diff_x := abs(o_pos.x - d_pos.x)
			diff_y := abs(o_pos.y - d_pos.y)
			if diff_x + diff_y <= 20 &&
			   o_cost + d_cost + diff_x + diff_y <=
				   baseline - input.min_cheats {
				acc += 1
			}
		}
	}
	return fmt.aprintf("%v", acc)
}

run :: proc(
	content: ^string,
	part1, part2: bool,
	expected_part1, expected_part2: string,
) -> bool {
	input := read_input(content)
	input.min_cheats = 2
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

	input_test_1 := `###############
#...#...#.....#
#.#.#.#.#.###.#
#S#...#.#.#...#
#######.#.#.###
#######.#.#...#
#######.#.###.#
###..E#...#...#
###.#######.###
#...###...#...#
#.#####.#.###.#
#.#...#.#.#...#
#.#.#.#.#.#.###
#...#...#...###
###############`

	if !run(&input_test_1, true, false, "44", "-1") {
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
	input.min_cheats = 100
	// fmt.println(input)

	part1 := part1_solve(input)
	fmt.println("Part1:", part1)
	part2 := part2_solve(input)
	fmt.println("Part2:", part2)
}
