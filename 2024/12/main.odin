package main

import "core:fmt"
import "core:os"
import "core:strings"

InputT :: [dynamic][dynamic]rune
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	for line in strings.split_lines_iterator(content) {
		row := [dynamic]rune{}
		for cell in line {
			append(&row, cell)
		}
		append(&input, row)
	}

	return input
}

GardenPlot :: struct {
	id:    rune,
	cells: map[[2]int]struct {},
}

DIR := [?][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}
DIR2 := [?][2]int {
	{-1, 0},
	{0, 1},
	{1, 0},
	{0, -1},
	{-1, -1},
	{-1, 1},
	{1, 1},
	{1, -1},
}

get_garden_plots :: proc(input: InputT, pos: [2]int) -> map[[2]int]struct {} {
	seen_borders := make(map[[2]int]struct {})
	seen_borders[pos] = {}

	ROW := len(input)
	COL := len(input[0])

	next_move_was_found := [dynamic][2]int{pos}
	for len(next_move_was_found) > 0 {
		current_border_cell := pop_front(&next_move_was_found)

		for d in DIR {
			new_pos := current_border_cell + d
			if new_pos.x < 0 ||
			   new_pos.x >= ROW ||
			   new_pos.y < 0 ||
			   new_pos.y >= COL {
				continue
			}
			if input[new_pos.x][new_pos.y] !=
			   input[current_border_cell.x][current_border_cell.y] {
				continue
			}
			if new_pos in seen_borders {
				continue
			}
			seen_borders[new_pos] = {}
			append(&next_move_was_found, new_pos)
		}
	}

	return seen_borders
}

get_gardens_plots :: proc(input: InputT) -> [dynamic]GardenPlot {
	garden_plots := make([dynamic]GardenPlot)
	seen_borders := make(map[[2]int]struct {})

	ROW := len(input)
	COL := len(input[0])

	for r in 0 ..< ROW {
		for c in 0 ..< COL {
			pos := [2]int{r, c}
			if pos in seen_borders {
				continue
			}

			plots := get_garden_plots(input, pos)
			for plot in plots {
				seen_borders[plot] = {}
			}
			append(&garden_plots, GardenPlot{input[r][c], plots})
		}
	}
	return garden_plots
}

part1_solve :: proc(input: InputT) -> OutputT {
	garden_plots := get_gardens_plots(input)

	// fmt.println(garden_plots)

	acc := 0
	for garden_plot in garden_plots {
		area := len(garden_plot.cells)
		perimeter := 4 * area

		for cell in garden_plot.cells {
			for d in DIR {
				new_pos := cell + d
				if new_pos in garden_plot.cells {
					perimeter -= 1
				}
			}
		}

		// fmt.println(garden_plot.id, perimeter, area)

		acc += perimeter * area
	}

	return fmt.aprintf("%v", acc)
}

get_border :: proc(pos: map[[2]int]struct {}) -> map[[2]int]struct {} {
	border := make(map[[2]int]struct {})

	for cell in pos {
		for d in DIR2 {
			new_pos := cell + d
			if !(new_pos in pos) {
				border[new_pos] = {}
			}
		}
	}
	return border
}

get_neighbors :: proc(
	pos: [2]int,
	border: map[[2]int]struct {},
	seen_border: map[[2]int]struct {},
) -> int {
	neigborgs := 0
	for d in DIR2 {
		new_pos := pos + d
		if new_pos in border && !(new_pos in seen_border) {
			neigborgs += 1
		}
	}
	return neigborgs
}

get_neighbors_pos :: proc(
	pos: [2]int,
	border: map[[2]int]struct {},
) -> map[[2]int]struct {} {
	neigborgs := make(map[[2]int]struct {})
	for d in DIR2 {
		new_pos := pos + d
		if new_pos in border {
			neigborgs[new_pos] = {}
		}
	}
	return neigborgs
}

common_cells :: proc(
	left: map[[2]int]struct {},
	right: map[[2]int]struct {},
) -> int {
	common_garden_cells := 0
	for l in left {
		if l in right {
			common_garden_cells += 1
		}
	}
	return common_garden_cells
}

part2_solve_helper :: proc(input: InputT, garden_plot: GardenPlot) -> int {
	border := get_border(garden_plot.cells)
	defer delete(border)
	// fmt.println("id:", garden_plot.id)
	// fmt.println("border:", border)

	all_sides := 0
	default := -1
	border_acc_seen := make(map[[2]int]struct {})
	defer delete(border_acc_seen)

	for border_cell in border {
		if border_cell in border_acc_seen {
			continue
		}

		neighbors_border := get_neighbors_pos(border_cell, garden_plot.cells)
		defer delete(neighbors_border)
		assert(len(neighbors_border) > 0)
		// fmt.println("neighbors_border:", neighbors_border)

		sides := 0
		last_dir := default
		start_dir := default
		current_border_cell := border_cell
		seen_borders := make(map[[2]int]struct {})
		defer delete(seen_borders)
		// fmt.println("start:", border_cell)
		seen_borders[current_border_cell] = {}
		for {
			next_move_was_found := false
			for dir_index in 0 ..< len(DIR) {
				current_dir_index := last_dir
				if current_dir_index < 0 {
					current_dir_index = 0
				}
				current_dir_index = (dir_index + current_dir_index) % len(DIR)
				new_pos := current_border_cell + DIR[current_dir_index]
				if (new_pos in border && !(new_pos in seen_borders)) ||
				   (new_pos == border_cell && sides > 0) {
					new_neighbors_border := get_neighbors_pos(
						new_pos,
						garden_plot.cells,
					)
					common_garden_cells := common_cells(
						new_neighbors_border,
						neighbors_border,
					)
					if common_garden_cells == 0 {
						continue
					}

					next_move_was_found = true
					if current_dir_index != last_dir {
						if last_dir != default {
							// fmt.println("turn:", current_border_cell, new_pos)
							sides += 1
						} else {
							start_dir = current_dir_index
						}
						last_dir = current_dir_index
					}
					current_border_cell = new_pos
					seen_borders[current_border_cell] = {}
					neighbors_border = new_neighbors_border
					break
				}
			}
			if !next_move_was_found {
				if last_dir != start_dir && sides > 0 {
					sides += 1
				}
				// fmt.println("seen_borders:", seen_borders)
				break
			}
		}
		for s in seen_borders {
			border_acc_seen[s] = {}
		}
		all_sides += sides
	}

	// fmt.println(garden_plot.id, all_sides)
	return all_sides
}

print_grid :: proc(input: InputT) {
	for row in input {
		fmt.println(row)
	}
}

part2_solve :: proc(input: InputT) -> OutputT {
	ROW := len(input)
	COL := len(input[0])

	modified_input := InputT{}
	resize(&modified_input, ROW * 2)
	for i in 0 ..< len(modified_input) {
		resize(&modified_input[i], COL * 2)
	}

	for r in 0 ..< ROW * 2 {
		for c in 0 ..< COL * 2 {
			modified_input[r][c] = input[r / 2][c / 2]
		}
	}
	// print_grid(modified_input)

	acc := 0
	modified_garden_plots := get_gardens_plots(modified_input)
	for garden_plot in modified_garden_plots {
		area := len(garden_plot.cells) / 4

		sides := part2_solve_helper(input, garden_plot)
		// fmt.println("id:", garden_plot.id, "area:", area, ", sides: ", sides)
		acc += area * sides
	}

	return fmt.aprintf("%v", acc)
}

run :: proc(
	content: ^string,
	part1, part2: bool,
	expected_part1, expected_part2: string,
	test_name: string,
) -> bool {
	input := read_input(content)
	// fmt.println(input)

	if part1 {
		part1_solution := part1_solve(input)
		if part1_solution != expected_part1 {
			fmt.println(
				"---------------",
				test_name,
				"Part1 - Expected fail: (calculated)",
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
				"---------------",
				test_name,
				"Part2 - Expected fail: (calculated)",
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

	input_test_1 := `AAAA
BBCD
BBCC
EEEC`

	if !run(&input_test_1, true, true, "140", "80", "1") {
		bad += 1
	}

	input_test_2 := `OOOOO
OXOXO
OOOOO
OXOXO
OOOOO`

	if !run(&input_test_2, true, true, "772", "436", "2") {
		bad += 1
	}

	input_test_3 := `RRRRIICCFF
RRRRIICCCF
VVRRRCCFFF
VVRCCCJFFF
VVVVCJJCFE
VVIVCCJJEE
VVIIICJJEE
MIIIIIJJEE
MIIISIJEEE
MMMISSJEEE`

	if !run(&input_test_3, true, true, "1930", "1206", "3") {
		bad += 1
	}

	input_test_4 := `EEEEE
EXXXX
EEEEE
EXXXX
EEEEE`

	if !run(&input_test_4, false, true, "0", "236", "4") {
		bad += 1
	}

	input_test_5 := `AAAAAA
AAABBA
AAABBA
ABBAAA
ABBAAA
AAAAAA`

	if !run(&input_test_5, false, true, "0", "368", "5") {
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
