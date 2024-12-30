package main

import "core:fmt"
import "core:os"
import "core:strings"

InputT :: struct {
	map_warehouse: [dynamic][dynamic]rune,
	movements:     [dynamic]rune,
	start_pos:     [2]int,
}
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	map_warehouse, _, movements := strings.partition(content^, "\n\n")
	x := 0
	for row in strings.split_lines_iterator(&map_warehouse) {
		row_cells := [dynamic]rune{}
		for cell, y in row {
			if cell == '@' {
				input.start_pos = [2]int{x, y}
			}
			append(&row_cells, cell)
		}
		append(&input.map_warehouse, row_cells)
		x += 1
	}

	for mov_lines in strings.split_lines_iterator(&movements) {
		for move in mov_lines {
			append(&input.movements, move)
		}
	}

	return input
}

DIR := [?][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}

move_in_direction :: proc(
	dir_index: int,
	current: ^[2]int,
	grid: ^[dynamic][dynamic]rune,
) {
	new_pos := current^ + DIR[dir_index]
	if grid[new_pos.x][new_pos.y] == '#' {
		return
	}
	if grid[new_pos.x][new_pos.y] == '.' {
		grid[current.x][current.y] = '.'
		grid[new_pos.x][new_pos.y] = '@'
		current^ = new_pos
		return
	}
	move_box_pos := new_pos
	for grid[move_box_pos.x][move_box_pos.y] == 'O' {
		move_box_pos += DIR[dir_index]
	}
	if grid[move_box_pos.x][move_box_pos.y] == '#' {
		return
	}
	assert(grid[move_box_pos.x][move_box_pos.y] == '.')
	for {
		// print_grid(grid^)
		grid[move_box_pos.x][move_box_pos.y] = 'O'
		move_box_pos -= DIR[dir_index]
		if grid[move_box_pos.x][move_box_pos.y] == '@' {
			grid[move_box_pos.x][move_box_pos.y] = '.'
			move_box_pos += DIR[dir_index]
			grid[move_box_pos.x][move_box_pos.y] = '@'
			current^ = move_box_pos
			// print_gride(grid^)
			break
		}
	}
}

print_grid :: proc(grid: [dynamic][dynamic]rune) {
	for row in grid {
		for col in row {
			fmt.print(col)
		}
		fmt.println()
	}
	fmt.println()
	fmt.println()
}

part1_solve :: proc(input: InputT) -> OutputT {
	grid := [dynamic][dynamic]rune{}
	ROW := len(input.map_warehouse)
	COL := len(input.map_warehouse[0])
	resize(&grid, ROW)
	for &row, x in grid {
		resize(&row, COL)
		copy(grid[x][:], input.map_warehouse[x][:])
	}

	current := input.start_pos

	for move in input.movements {
		switch move {
		case '^':
			move_in_direction(0, &current, &grid)

		case '>':
			move_in_direction(1, &current, &grid)
		case 'v':
			move_in_direction(2, &current, &grid)
		case '<':
			move_in_direction(3, &current, &grid)
		}
	}

	// print_grid(grid)

	acc_gps_coord := 0
	for r in 0 ..< ROW {
		for c in 0 ..< COL {
			if grid[r][c] == 'O' {
				acc_gps_coord += 100 * r + c
			}
		}
	}
	return fmt.aprintf("%v", acc_gps_coord)
}

move_in_direction2 :: proc(
	dir_index: int,
	current: ^[2]int,
	grid: ^[dynamic][dynamic]rune,
) {
	new_pos := current^ + DIR[dir_index]
	if grid[new_pos.x][new_pos.y] == '#' {
		return
	}
	if grid[new_pos.x][new_pos.y] == '.' {
		grid[current.x][current.y] = '.'
		grid[new_pos.x][new_pos.y] = '@'
		current^ = new_pos
		return
	}

	if dir_index == 1 || dir_index == 3 {
		move_box_pos := new_pos
		assert(
			grid[move_box_pos.x][move_box_pos.y] == '[' ||
			grid[move_box_pos.x][move_box_pos.y] == ']',
		)
		for grid[move_box_pos.x][move_box_pos.y] == '[' ||
		    grid[move_box_pos.x][move_box_pos.y] == ']' {
			move_box_pos += DIR[dir_index] * [2]int{1, 2}
		}
		if grid[move_box_pos.x][move_box_pos.y] == '#' {
			return
		}
		assert(grid[move_box_pos.x][move_box_pos.y] == '.')
		for {
			// print_grid(grid^)
			move_box_pos1 := move_box_pos - DIR[dir_index]
			if move_box_pos1.y < move_box_pos.y {
				grid[move_box_pos1.x][move_box_pos1.y] = '['
				grid[move_box_pos.x][move_box_pos.y] = ']'
			} else {
				grid[move_box_pos1.x][move_box_pos1.y] = ']'
				grid[move_box_pos.x][move_box_pos.y] = '['
			}
			move_box_pos = move_box_pos1 - DIR[dir_index]
			np := move_box_pos - DIR[dir_index]
			if grid[np.x][np.y] == '@' {
				grid[np.x][np.y] = '.'
				np += DIR[dir_index]
				grid[np.x][np.y] = '@'
				current^ = np
				// print_gride(grid^)
				break
			}
		}
	} else {
		move_box_pos := [dynamic][dynamic][2]int{}
		current_box_pos := [dynamic][2]int{}
		assert(
			grid[new_pos.x][new_pos.y] == '[' ||
			grid[new_pos.x][new_pos.y] == ']',
		)
		if grid[new_pos.x][new_pos.y] == '[' {
			append(&current_box_pos, new_pos)
			append(&current_box_pos, new_pos + [2]int{0, 1})
		} else {
			append(&current_box_pos, new_pos)
			append(&current_box_pos, new_pos + [2]int{0, -1})
		}
		append(&move_box_pos, current_box_pos)

		for {
			new_move_box_row := make(map[[2]int]struct {})
			defer delete(new_move_box_row)
			for i in 0 ..< len(move_box_pos[len(move_box_pos) - 1]) {
				prev_move_current := move_box_pos[len(move_box_pos) - 1][i]
				move_current := prev_move_current + DIR[dir_index]
				if grid[move_current.x][move_current.y] == '#' {
					return
				}
				if grid[move_current.x][move_current.y] == '.' {
					continue
				}
				assert(
					grid[move_current.x][move_current.y] == '[' ||
					grid[move_current.x][move_current.y] == ']',
				)
				if grid[move_current.x][move_current.y] == '[' {
					new_move_box_row[move_current] = {}
					new_move_box_row[move_current + [2]int{0, 1}] = {}
				} else {
					new_move_box_row[move_current] = {}
					new_move_box_row[move_current + [2]int{0, -1}] = {}
				}
			}
			if len(new_move_box_row) == 0 {
				break
			}
			new_move_box_row_arr := [dynamic][2]int{}
			for new_move_box_row_elem in new_move_box_row {
				append(&new_move_box_row_arr, new_move_box_row_elem)
			}
			append(&move_box_pos, new_move_box_row_arr)
		}

		for ii in 0 ..< len(move_box_pos) {
			for jj in 0 ..< len(move_box_pos[len(move_box_pos) - 1 - ii]) {
				current_box := move_box_pos[len(move_box_pos) - 1 - ii][jj]
				new_box := current_box + DIR[dir_index]
				grid[new_box.x][new_box.y] = grid[current_box.x][current_box.y]
				grid[current_box.x][current_box.y] = '.'
			}
		}
		grid[current.x][current.y] = '.'
		current^ += DIR[dir_index]
		grid[current.x][current.y] = '@'
	}
}

part2_solve :: proc(input: InputT) -> OutputT {
	grid := [dynamic][dynamic]rune{}
	ROW := len(input.map_warehouse)
	COL := len(input.map_warehouse[0])
	resize(&grid, ROW)
	for &row in grid {
		resize(&row, COL * 2)
	}
	for r in 0 ..< ROW {
		for c in 0 ..< COL {
			if input.map_warehouse[r][c] == '#' {
				grid[r][2 * c] = '#'
				grid[r][2 * c + 1] = '#'
			} else if input.map_warehouse[r][c] == 'O' {
				grid[r][2 * c] = '['
				grid[r][2 * c + 1] = ']'
			} else if input.map_warehouse[r][c] == '.' {
				grid[r][2 * c] = '.'
				grid[r][2 * c + 1] = '.'
			} else if input.map_warehouse[r][c] == '@' {
				grid[r][2 * c] = '@'
				grid[r][2 * c + 1] = '.'
			}
		}
	}

	current := input.start_pos * [2]int{1, 2}

	for move in input.movements {
		// print_grid(grid)
		// fmt.println(current, move)

		switch move {
		case '^':
			move_in_direction2(0, &current, &grid)

		case '>':
			move_in_direction2(1, &current, &grid)
		case 'v':
			move_in_direction2(2, &current, &grid)
		case '<':
			move_in_direction2(3, &current, &grid)
		}
	}

	// print_grid(grid)

	acc_gps_coord := 0
	for r in 0 ..< ROW {
		for c in 0 ..< COL * 2 {
			if grid[r][c] == '[' {
				acc_gps_coord += 100 * r + c
			}
		}
	}
	return fmt.aprintf("%v", acc_gps_coord)
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

	input_test_0 := `#######
#...#.#
#.....#
#..OO@#
#..O..#
#.....#
#######

<vv<<^^<<^^`

	if !run(&input_test_0, false, true, "-1", "618") {
		bad += 1
	}

	input_test_1 := `########
#..O.O.#
##@.O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

<^^>>>vv<v>>v<<`

	if !run(&input_test_1, true, false, "2028", "-1") {
		bad += 1
	}

	input_test_2 := `##########
#..O..O.O#
#......O.#
#.OO..O.O#
#..O@..O.#
#O#..O...#
#O..O..O.#
#.OO.O.OO#
#....O...#
##########

<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^
vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v
><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<
<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^
^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><
^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^
>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^
<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>
^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>
v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^`

	if !run(&input_test_2, true, true, "10092", "9021") {
		bad += 1
	}

	return bad
}

main :: proc() {
	// fail_tests := run_tests()
	// if fail_tests != 0 {
	// 	fmt.println("Some tests fail:", fail_tests)
	// 	return
	// }

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
