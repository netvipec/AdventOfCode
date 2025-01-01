package main

import "core:fmt"
import "core:os"
import "core:strings"

print_grid :: proc(grid: ^[dynamic]string) {
	for row in grid {
		fmt.println(row)
	}
}

print_grid_pos :: proc(grid: ^[dynamic]string, pos: ^map[[2]int]struct {}) {
	for row, x in grid {
		for col, y in row {
			p := [?]int{x, y}
			if p in pos {
				fmt.print("O")
			} else {
				fmt.print(col)
			}
		}
		fmt.println()
	}
	fmt.println()
}

directions :: [4][2]int{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}

part1_solve :: proc(grid: ^[dynamic]string, start: ^[2]int) -> int {
	visited := make(map[[2]int]struct {})
	defer delete(visited)
	origins := make(map[[2]int]struct {})
	defer delete(origins)
	origins[start^] = {}

	for _ in 0 ..< 64 {
		// fmt.println(origins)
		for origin in origins {
			for d in directions {
				x, y := origin[0] + d[0], origin[1] + d[1]
				if x < 0 ||
				   x >= len(grid[0]) ||
				   y < 0 ||
				   y >= len(grid) ||
				   grid[x][y] == '#' {
					continue
				}

				visited[[2]int{x, y}] = {}
			}
		}
		origins, visited = visited, origins
		// print_grid_pos(grid, &visited)
		clear(&visited)
	}
	return len(origins)
}

part2_solve_long :: proc(grid: ^[dynamic]string, start: ^[2]int) -> int {
	grid_pos := make(map[[2]int]struct {})
	defer delete(grid_pos)
	max_x := len(grid)
	max_y := len(grid[0])

	for row, x in grid {
		for col, y in row {
			if col == '#' {
				grid_pos[[?]int{x, y}] = {}
			}
		}
	}
	fmt.println(grid_pos)

	visited := make(map[[2]int]struct {})
	defer delete(visited)
	origins := make(map[[2]int]struct {})
	defer delete(origins)
	origins[start^] = {}
	seen := make(map[[2]int]struct {})
	defer delete(seen)

	sum: [2]int = {0, 0}

	max_steps := 1000
	for steps in 0 ..< max_steps {
		// fmt.println(origins)
		for origin in origins {
			for d in directions {
				x, y := origin[0] + d[0], origin[1] + d[1]
				new_x := x + (-x / max_x + 1) * max_x if x < 0 else x
				new_y := y + (-y / max_y + 1) * max_y if y < 0 else y
				p := [?]int{new_x % max_x, new_y % max_y}
				if p in grid_pos {
					continue
				}

				new_p := [?]int{x, y}
				if new_p in seen {
					continue
				}
				seen[new_p] = {}

				visited[[2]int{x, y}] = {}
			}
		}
		sum[steps % 2] += len(visited)
		fmt.println(steps, len(visited), sum[steps % 2])
		origins, visited = visited, origins
		// print_grid_pos(grid, &visited)
		clear(&visited)
	}
	// fmt.println(origins)
	return sum[(max_steps + 1) % 2]
}

part2_solve :: proc(grid: ^[dynamic]string, start: ^[2]int) -> int {
	// steps with values 65 + 131 * x
	// 65 = 3778 
	// 195 = 33833
	// 326 = 93864
	// 457 = 183315
	// 588 = 303854
	// 719 = 453813
	// 850 = 633748

	// a + b + 3778 = 33833
	// a + b = 30055

	// (30055 - b)*4 + b*2 + 3778 = 93864
	// 120220 - 4b + 2b + 3778 = 93864
	// 30134 = 2b
	// a = 14988
	// b = 15067

	size := len(grid)
	a :: 14988
	b :: 15067
	c :: 3778
	target :: 26501365

	index := (target - 65) / size

	return a * index * index + b * index + c
}

main :: proc() {
	data := os.read_entire_file("input") or_else os.exit(1)
	defer delete(data)
	s := string(data)
	grid := [dynamic]string{}
	defer delete(grid)
	start := [2]int{0, 0}
	r := 0
	for line in strings.split_lines_iterator(&s) {
		if c := strings.index_byte(line, 'S'); c != -1 {
			start = {r, c}
		}
		append(&grid, line)
		r += 1
	}

	// fmt.println(start)
	// print_grid(&grid)
	part1 := part1_solve(&grid, &start)
	fmt.println("Part1:", part1)
	part2 := part2_solve(&grid, &start)
	fmt.println("Part2:", part2)
}
