package main

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"

Robots :: struct {
	pos: [2]int,
	vel: [2]int,
}

InputT :: struct {
	robots: [dynamic]Robots,
	size:   [2]int,
}
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	for line in strings.split_lines_iterator(content) {
		pos, _, vel := strings.partition(line, " ")
		pos_y, _, pos_x := strings.partition(pos[2:], ",")
		vel_y, _, vel_x := strings.partition(vel[2:], ",")

		append(
			&input.robots,
			Robots {
				[2]int{strconv.atoi(pos_x), strconv.atoi(pos_y)},
				[2]int{strconv.atoi(vel_x), strconv.atoi(vel_y)},
			},
		)
	}

	return input
}

print :: proc(robots: [dynamic]Robots, size: [2]int) {
	grid := [dynamic][dynamic]int{}
	resize(&grid, size.x)
	for &r in grid {
		resize_dynamic_array(&r, size.y)
	}
	for robot in robots {
		grid[robot.pos.x][robot.pos.y] += 1
	}

	for r in 0 ..< size.x {
		for c in 0 ..< size.y {
			if r == size.x / 2 || c == size.y / 2 {
				fmt.print("(", grid[r][c], ")")
			} else {
				fmt.print(" ", grid[r][c], " ")
			}
		}
		fmt.printfln("")
	}
	fmt.printfln("")
	fmt.printfln("")
}

print2 :: proc(robots: [dynamic]Robots, size: [2]int) {
	grid := [dynamic][dynamic]int{}
	resize(&grid, size.x)
	for &r in grid {
		resize_dynamic_array(&r, size.y)
	}
	for robot in robots {
		grid[robot.pos.x][robot.pos.y] += 1
	}

	for r in 0 ..< size.x {
		for c in 0 ..< size.y {
			if grid[r][c] > 0 {
				fmt.print('0')
			} else {
				fmt.print(' ')
			}
		}
		fmt.printfln("")
	}
	fmt.printfln("")
	fmt.printfln("")
}

part1_solve :: proc(input: InputT) -> OutputT {
	current_robots := [dynamic]Robots{}
	resize(&current_robots, len(input.robots))
	copy(current_robots[:], input.robots[:])

	for _ in 0 ..< 100 {
		new_robots := [dynamic]Robots{}
		for robot in current_robots {
			new_pos := robot.pos + robot.vel
			for new_pos.x < 0 {
				new_pos.x += input.size.x
			}
			for new_pos.y < 0 {
				new_pos.y += input.size.y
			}
			new_pos %= input.size
			append(&new_robots, Robots{new_pos, robot.vel})
		}
		copy(current_robots[:], new_robots[:])

		// print(current_robots, input.size)
	}

	middle := input.size / 2
	quadrant_safe_robots := make(map[[2]int]int)
	defer delete(quadrant_safe_robots)
	for &robot in current_robots {
		if robot.pos.x == middle.x || robot.pos.y == middle.y {
			continue
		}

		if robot.pos.x > middle.x {
			robot.pos.x -= 1
		}
		if robot.pos.y > middle.y {
			robot.pos.y -= 1
		}
		quadrant := robot.pos / middle
		quadrant_safe_robots[quadrant] += 1
	}

	// fmt.println(quadrant_safe_robots)

	acc := 1
	for _, safe_robots in quadrant_safe_robots {
		acc *= safe_robots
	}

	return fmt.aprintf("%v", acc)
}

is_frame_present :: proc(robots: [dynamic]Robots, size: [2]int) -> bool {
	robots_pos := make(map[[2]int]struct {})
	defer delete(robots_pos)
	for robot in robots {
		robots_pos[robot.pos] = {}
	}

	for x in 0 ..< size.x / 2 {
		counter := 0
		max_counter := 0
		for y in 0 ..< size.y {
			pos := [2]int{x, y}
			if pos in robots_pos {
				counter += 1
			} else {
				if counter > max_counter {
					max_counter = counter
				}
				counter = 0
			}
		}
		if max_counter > 30 {
			return true
		}
	}

	return false
}

part2_solve :: proc(input: InputT) -> OutputT {
	current_robots := [dynamic]Robots{}
	resize(&current_robots, len(input.robots))
	copy(current_robots[:], input.robots[:])

	for s in 0 ..< 10000 {
		new_robots := [dynamic]Robots{}
		for robot in current_robots {
			new_pos := robot.pos + robot.vel
			for new_pos.x < 0 {
				new_pos.x += input.size.x
			}
			for new_pos.y < 0 {
				new_pos.y += input.size.y
			}
			new_pos %= input.size
			append(&new_robots, Robots{new_pos, robot.vel})
		}

		copy(current_robots[:], new_robots[:])

		good := is_frame_present(current_robots, input.size)
		if good {
			print2(current_robots, input.size)
			return fmt.aprintf("%v", s + 1)
		}
	}

	return fmt.aprintf("%v", 0)
}

run :: proc(
	content: ^string,
	part1, part2: bool,
	expected_part1, expected_part2: string,
) -> bool {
	input := read_input(content)
	input.size = [2]int{7, 11}
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

	input_test_1 := `p=0,4 v=3,-3
p=6,3 v=-1,-3
p=10,3 v=-1,2
p=2,0 v=2,-1
p=0,0 v=1,3
p=3,0 v=-2,-2
p=7,6 v=-1,-3
p=3,0 v=-1,-2
p=9,3 v=2,3
p=7,3 v=-1,2
p=2,4 v=2,-3
p=9,5 v=-3,-3`

	if !run(&input_test_1, true, false, "12", "-1") {
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
	input.size = [2]int{103, 101}
	// fmt.println(input)

	part1 := part1_solve(input)
	fmt.println("Part1:", part1)
	part2 := part2_solve(input)
	fmt.println("Part2:", part2)
}
