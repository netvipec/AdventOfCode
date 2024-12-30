package main

import "core:fmt"
import "core:os"
import "core:strings"
import "core:unicode/utf8"

InputT :: [dynamic][dynamic]rune
OutputT :: string

BfsData :: struct {
	pos:  [2]int,
	path: [dynamic]rune,
}

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	for line in strings.split_lines_iterator(content) {
		l := [dynamic]rune{}
		for c in line {
			append(&l, c)
		}
		append(&input, l)
	}

	return input
}

DIR := [?][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}} // up right down left
DIR_move := [?]rune{'^', '>', 'v', '<'} // up right down left

numerical_grid := [dynamic][3]rune {
	{'7', '8', '9'},
	{'4', '5', '6'},
	{'1', '2', '3'},
	{' ', '0', 'A'},
}

directional_grid := [dynamic][3]rune{{' ', '^', 'A'}, {'<', 'v', '>'}}

bfs :: proc(
	start: [2]int,
	end: [2]int,
	grid: ^[dynamic][3]rune,
) -> [dynamic][dynamic]rune {
	ROW := len(grid)
	COL := len(grid[0])

	process := [dynamic]BfsData{}
	seen := make(map[[2]int]int)
	defer delete(seen)

	best_len := -1
	result := [dynamic][dynamic]rune{}

	append(&process, BfsData{start, {}})
	seen[start] = 0
	for len(process) > 0 {
		current := pop_front(&process)
		if current.pos == end {
			if best_len < 0 || len(current.path) == best_len {
				best_len = len(current.path)
				append(&current.path, 'A')
				append(&result, current.path)
				continue
			} else {
				return result
			}
		}
		if best_len > 0 {
			continue
		}
		for id in 0 ..< len(DIR) {
			new_pos := current.pos + DIR[id]
			if new_pos.x < 0 ||
			   new_pos.x >= ROW ||
			   new_pos.y < 0 ||
			   new_pos.y >= COL {
				continue
			}
			if grid[new_pos.x][new_pos.y] == ' ' {
				continue
			}
			seen_elem, seen_found := &seen[new_pos]
			if seen_found && seen_elem^ < len(current.path) && new_pos != end {
				// fmt.println(current.pos, new_pos, current.path)
				continue
			}
			seen[new_pos] = len(current.path) + 1
			new_path := [dynamic]rune{}
			resize(&new_path, len(current.path) + 1)
			copy(new_path[:len(current.path)], current.path[:])
			new_path[len(current.path)] = DIR_move[id]
			append(&process, BfsData{new_pos, new_path})
		}
	}
	return result
}

get_best_paths :: proc(
	grid: ^[dynamic][3]rune,
) -> map[[2]rune][dynamic][dynamic]rune {
	best_paths := make(map[[2]rune][dynamic][dynamic]rune)

	for x1 in 0 ..< len(grid) {
		for y1 in 0 ..< len(grid[0]) {
			for x2 in 0 ..< len(grid) {
				for y2 in 0 ..< len(grid[0]) {
					if grid[x1][y1] == ' ' || grid[x2][y2] == ' ' {
						continue
					}
					best_path := bfs([2]int{x1, y1}, [2]int{x2, y2}, grid)
					// fmt.println([2]rune{grid[x1][y1], grid[x2][y2]}, best_path)
					best_paths[[2]rune{grid[x1][y1], grid[x2][y2]}] = best_path
				}
			}
		}
	}
	return best_paths
}

get_possible_paths :: proc(
	code: ^[dynamic]rune,
	best_paths: ^map[[2]rune][dynamic][dynamic]rune,
) -> [dynamic][dynamic]rune {
	possible_paths := make([dynamic][dynamic][dynamic]rune)
	prev_char := 'A'
	for char in code {
		if prev_char == char {
			append(&possible_paths, [dynamic][dynamic]rune{{'A'}})
			continue
		}
		paths, found := &best_paths[[2]rune{prev_char, char}]
		assert(found)
		// fmt.println(prev_char, char, paths)

		prev_char = char
		append(&possible_paths, [dynamic][dynamic]rune{})
		for bp in paths {
			ap := [dynamic]rune{}
			for bpe in bp {
				append(&ap, bpe)
			}
			append(&possible_paths[len(possible_paths) - 1], ap)
		}
	}

	solutions := [dynamic][dynamic]rune{}
	new_solutions := [dynamic][dynamic]rune{}
	append(&solutions, [dynamic]rune{})
	for possible_path in possible_paths {
		for solution in solutions {
			for path in possible_path {
				new_path := [dynamic]rune{}
				append(&new_path, ..solution[:])
				append(&new_path, ..path[:])
				append(&new_solutions, new_path)
			}
		}
		resize(&solutions, len(new_solutions))
		copy(solutions[:], new_solutions[:])
		clear(&new_solutions)
	}
	// fmt.println(solutions)
	return solutions
}

get_sizes :: proc(
	possible_paths: ^[dynamic][dynamic]rune,
) -> (
	int,
	[dynamic]int,
) {
	best_size := max(int)
	sizes := [dynamic]int{}
	mult := 1
	for pp in possible_paths {
		size := len(pp)
		for i in 1 ..< len(pp) {
			if pp[i - 1] == pp[i] {
				size -= mult
				mult <<= 1
			} else {
				mult = 1
			}
		}
		append(&sizes, size)
		best_size = min(size, best_size)
	}
	return best_size, sizes
}

get_solution :: proc(
	code: ^[dynamic]rune,
	depth: int,
	best_paths_directional: ^map[[2]rune][dynamic][dynamic]rune,
) -> int {
	// fmt.println(depth)
	possible_paths := get_possible_paths(code, best_paths_directional)
	defer delete(possible_paths)

	best_size, sizes := get_sizes(&possible_paths)

	min_complexity := max(int)
	for j in 0 ..< len(possible_paths) {
		pp := possible_paths[j]
		if sizes[j] != best_size {
			continue
		}
		if depth > 0 {
			min_complexity = min(
				min_complexity,
				get_solution(&pp, depth - 1, best_paths_directional),
			)
		} else {
			min_complexity = min(min_complexity, len(pp))
		}
	}
	return min_complexity
}

part1_solve :: proc(input: InputT) -> OutputT {
	best_paths_numerical := get_best_paths(&numerical_grid)
	defer delete(best_paths_numerical)

	best_paths_directional := get_best_paths(&directional_grid)
	defer delete(best_paths_directional)

	// fmt.println(best_paths_numerical)
	// fmt.println(best_paths_directional)

	acc := 0
	for &code in input {
		min_complexity := max(int)
		n := 0
		for r in code {
			if '0' <= r && r <= '9' {
				n = n * 10 + cast(int)(r - '0')
			}
		}
		// fmt.println(code, n)

		possible_paths_1 := get_possible_paths(&code, &best_paths_numerical)
		defer delete(possible_paths_1)
		// fmt.println("len1:", len(possible_paths_1))
		// for pp in possible_paths_1 {
		// 	fmt.println(pp)
		// }

		best_size1, sizes1 := get_sizes(&possible_paths_1)
		// fmt.println("best_size1:", best_size1)

		for i in 0 ..< len(possible_paths_1) {
			pp1 := possible_paths_1[i]
			if sizes1[i] != best_size1 {
				continue
			}
			// fmt.println("size1:", len(pp1))
			min_complexity = min(
				min_complexity,
				get_solution(&pp1, 1, &best_paths_directional) * n,
			)
		}
		acc += min_complexity
	}

	return fmt.aprintf("%v", acc)
}

CacheKey :: struct {
	buttons: string,
	depth:   int,
}
cache := make(map[CacheKey]int)
best_paths_directional := get_best_paths(&directional_grid)

press_button :: proc(buttons: string, depth: int) -> int {
	if depth == 0 {
		return len(buttons)
	}
	if buttons == "A" {
		return 1
	}
	key := CacheKey{buttons, depth}
	value, found := &cache[key]
	if found {
		return value^
	}

	all_path_size := 0
	prev_button := 'A'
	for button in buttons {
		best_path_size := max(int)
		for path in best_paths_directional[[2]rune{prev_button, button}] {
			current_path_size := press_button(
				utf8.runes_to_string(path[:]),
				depth - 1,
			)
			best_path_size = min(best_path_size, current_path_size)
		}
		prev_button = button
		all_path_size += best_path_size
	}
	cache[key] = all_path_size

	return all_path_size
}

part2_solve :: proc(input: InputT) -> OutputT {
	best_paths_numerical := get_best_paths(&numerical_grid)
	defer delete(best_paths_numerical)

	depth := 25

	acc := 0
	for &code in input {
		n := 0
		for r in code {
			if '0' <= r && r <= '9' {
				n = n * 10 + cast(int)(r - '0')
			}
		}

		possible_paths_1 := get_possible_paths(&code, &best_paths_numerical)
		defer delete(possible_paths_1)

		best_size := max(int)
		for path in possible_paths_1 {
			// fmt.println("depth:", depth, "path:", path)
			size := press_button(utf8.runes_to_string(path[:]), depth)
			// fmt.println(size, path)
			best_size = min(best_size, size)
		}
		// fmt.println("best size:", best_size, "code:", code)
		acc += best_size * n
	}

	return fmt.aprintf("%v", acc)
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

	input_test_1 := `029A
980A
179A
456A
379A`

	if !run(&input_test_1, true, false, "126384", "-1") {
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
