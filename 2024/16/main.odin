package main

import "core:fmt"
import "core:os"
import "core:strings"

InputT :: struct {
	grid:  [dynamic][dynamic]rune,
	start: [2]int,
	end:   [2]int,
}
OutputT :: string

Cell :: struct {
	pos: [2]int,
	dir: int,
}

CellExt :: struct {
	cell:  Cell,
	cost:  int,
	tiles: [dynamic][2]int,
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

insert_best :: proc(process: ^[dynamic]CellExt, cell: CellExt) {
	for i in 0 ..< len(process) {
		if process[i].cost > cell.cost {
			inject_at(process, i, cell)
			return
		}
	}
	if len(process) == 0 {
		inject_at(process, 0, cell)
	} else {
		append(process, cell)
	}
}

print_grid :: proc(grid: [dynamic][dynamic]rune, cell: CellExt) {
	fmt.println(cell)
	for row, x in grid {
		for _, y in row {
			if cell.cell.pos.x == x && cell.cell.pos.y == y {
				switch cell.cell.dir {
				case 0:
					fmt.print('^')
				case 1:
					fmt.print('>')
				case 2:
					fmt.print('v')
				case 3:
					fmt.print('<')
				}
			} else {
				fmt.print(grid[x][y])
			}
		}
		fmt.println()
	}
	fmt.println()
	fmt.println()
}

part1_solve_helper :: proc(
	input: InputT,
	functor: proc(_: CellExt) -> (bool, OutputT),
) -> OutputT {
	process := [dynamic]CellExt{}
	seen := make(map[Cell]int)
	defer delete(seen)

	origin := CellExt{Cell{input.start, 3}, 0, {}}
	append(&origin.tiles, input.start)

	append(&process, origin)
	seen[origin.cell] = origin.cost
	for len(process) > 0 {
		current := pop_front(&process)
		// print_grid(input.grid, current)
		if current.cell.pos == input.end {
			stop, result := functor(current)
			if stop {
				return result
			}
		}

		new_pos := current.cell.pos + DIR[current.cell.dir]
		if input.grid[new_pos.x][new_pos.y] == '.' {
			new_cell := Cell{new_pos, current.cell.dir}
			seen_elem, seen_found := &seen[new_cell]
			if seen_found && seen_elem^ < current.cost + 1 {
				continue
			}
			seen[new_cell] = current.cost + 1
			new_tiles := [dynamic][2]int{}
			resize(&new_tiles, len(current.tiles) + 1)
			copy(new_tiles[:len(current.tiles)], current.tiles[:])
			new_tiles[len(current.tiles)] = new_cell.pos
			insert_best(
				&process,
				CellExt{new_cell, current.cost + 1, new_tiles},
			)
		}
		new_cell := Cell{current.cell.pos, (current.cell.dir + 1) % len(DIR)}
		seen_elem, seen_found := &seen[new_cell]
		if seen_found && seen_elem^ < current.cost + 1 {
			continue
		}
		seen[new_cell] = current.cost + 1000
		new_tiles := [dynamic][2]int{}
		resize(&new_tiles, len(current.tiles) + 1)
		copy(new_tiles[:len(current.tiles)], current.tiles[:])
		new_tiles[len(current.tiles)] = new_cell.pos
		insert_best(
			&process,
			CellExt{new_cell, current.cost + 1000, new_tiles},
		)

		new_cell2 := Cell{current.cell.pos, (current.cell.dir + 3) % len(DIR)}
		seen_elem2, seen_found2 := &seen[new_cell2]
		if seen_found2 && seen_elem2^ < current.cost + 1 {
			continue
		}
		seen[new_cell2] = current.cost + 1000
		new_tiles2 := [dynamic][2]int{}
		resize(&new_tiles2, len(current.tiles) + 1)
		copy(new_tiles2[:len(current.tiles)], current.tiles[:])
		new_tiles2[len(current.tiles)] = new_cell.pos
		insert_best(
			&process,
			CellExt{new_cell2, current.cost + 1000, new_tiles2},
		)
	}

	return fmt.aprintf("%v", 0)
}

part1_solve :: proc(input: InputT) -> OutputT {
	return part1_solve_helper(
		input,
		proc(current: CellExt) -> (bool, OutputT) {
			return true, fmt.aprintf("%v", current.cost)
		},
	)
}

best_solution := -1
visited_tiles := make(map[[2]int]struct {})

part2_solve :: proc(input: InputT) -> OutputT {
	best_solution = -1
	clear(&visited_tiles)
	return part1_solve_helper(
		input,
		proc(current: CellExt) -> (bool, OutputT) {
			if best_solution < 0 {
				best_solution = current.cost
			}
			if (current.cost == best_solution) {
				for tile in current.tiles {
					visited_tiles[tile] = {}
				}
			} else {
				return true, fmt.aprintf("%v", len(visited_tiles))
			}
			return false, ""
		},
	)
}

run :: proc(
	content: ^string,
	part1, part2: bool,
	expected_part1, expected_part2: string,
) -> bool {
	input := read_input(content)
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
#.......#....E#
#.#.###.#.###.#
#.....#.#...#.#
#.###.#####.#.#
#.#.#.......#.#
#.#.#####.###.#
#...........#.#
###.#.#####.#.#
#...#.....#.#.#
#.#.#.###.#.#.#
#.....#...#.#.#
#.###.#.#.#.#.#
#S..#.....#...#
###############`

	if !run(&input_test_1, true, true, "7036", "45") {
		bad += 1
	}

	input_test_2 := `#################
#...#...#...#..E#
#.#.#.#.#.#.#.#.#
#.#.#.#...#...#.#
#.#.#.#.###.#.#.#
#...#.#.#.....#.#
#.#.#.#.#.#####.#
#.#...#.#.#.....#
#.#.#####.#.###.#
#.#.#.......#...#
#.#.###.#####.###
#.#.#...#.....#.#
#.#.#.#####.###.#
#.#.#.........#.#
#.#.#.#########.#
#S#.............#
#################`

	if !run(&input_test_2, true, true, "11048", "64") {
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
	// fmt.println(input)

	part1 := part1_solve(input)
	fmt.println("Part1:", part1)
	part2 := part2_solve(input)
	fmt.println("Part2:", part2)
}
