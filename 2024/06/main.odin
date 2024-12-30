package main

import "core:fmt"
import "core:os"
import "core:strings"

TEST :: false

Position :: struct {
	pos: [2]int,
	dir: int,
}
InputT :: struct {
	grid:  [dynamic][dynamic]rune,
	start: Position,
}

OutputT :: string

DIR := [?][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	x := -1
	for line in strings.split_lines_iterator(content) {
		x += 1
		row := [dynamic]rune{}
		for cell, y in line {
			switch cell {
			case '^':
				input.start = {[2]int{x, y}, 0}
			case '>':
				input.start = {[2]int{x, y}, 1}
			case 'v':
				input.start = {[2]int{x, y}, 2}
			case '<':
				input.start = {[2]int{x, y}, 3}
			}
			append(&row, cell)
		}
		append(&input.grid, row)
	}

	return input
}

get_path :: proc(input: InputT) -> map[[2]int]struct {} {
	ROW := len(input.grid)
	COL := len(input.grid[0])

	path := make(map[[2]int]struct {})
	path[input.start.pos] = {}

	current := input.start
	for {
		new_pos := current.pos + DIR[current.dir]
		if new_pos.x < 0 ||
		   new_pos.x >= ROW ||
		   new_pos.y < 0 ||
		   new_pos.y >= COL {
			break
		}

		if input.grid[new_pos.x][new_pos.y] == '#' {
			current.dir = (current.dir + 1) % len(DIR)
		} else {
			current.pos = new_pos
			path[new_pos] = {}
		}
	}

	return path
}

part1_solve :: proc(input: InputT) -> OutputT {
	path := get_path(input)

	return fmt.aprintf("%v", len(path))
}

part2_solve :: proc(input: InputT) -> OutputT {
	path := get_path(input)

	input := input

	ROW := len(input.grid)
	COL := len(input.grid[0])

	cycle_count := 0
	for p in path {
		x := p.x
		y := p.y

		input.grid[x][y] = '#'
		defer input.grid[x][y] = '.'

		path := make(map[Position]struct {})
		path[input.start] = {}

		current := input.start
		cycle := false
		for {
			new_pos := current.pos + DIR[current.dir]
			if new_pos.x < 0 ||
			   new_pos.x >= ROW ||
			   new_pos.y < 0 ||
			   new_pos.y >= COL {
				break
			}

			if input.grid[new_pos.x][new_pos.y] == '#' {
				current.dir = (current.dir + 1) % len(DIR)
				if current in path {
					cycle = true
					break
				}
				path[current] = {}
			} else {
				current.pos = new_pos
				if current in path {
					cycle = true
					break
				}
				path[current] = {}
			}
		}
		if (cycle) {
			cycle_count += 1
		}
	}

	return fmt.aprintf("%v", cycle_count)
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
