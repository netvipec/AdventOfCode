package main

import "core:fmt"
import "core:math"
import "core:os"
import "core:strconv"
import "core:strings"

ClawMachine :: struct {
	buttonA, buttonB, prize: [2]int,
}

InputT :: [dynamic]ClawMachine
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	claw_machines := strings.split(content^, "\n\n")
	for claw_machine in claw_machines {
		claw_machine := strings.split(claw_machine, "\n")
		assert(len(claw_machine) == 3)

		equation_a := claw_machine[0][len("Button A: "):]
		a_x, _, a_y := strings.partition(equation_a, ", ")
		equation_b := claw_machine[1][len("Button B: "):]
		b_x, _, b_y := strings.partition(equation_b, ", ")
		prize := claw_machine[2][len("Prize: "):]
		prize_x, _, prize_y := strings.partition(prize, ", ")

		append(
			&input,
			ClawMachine {
				[2]int{strconv.atoi(a_x[2:]), strconv.atoi(a_y[2:])},
				[2]int{strconv.atoi(b_x[2:]), strconv.atoi(b_y[2:])},
				[2]int{strconv.atoi(prize_x[2:]), strconv.atoi(prize_y[2:])},
			},
		)
	}

	return input
}

part1_solve :: proc(input: InputT) -> OutputT {
	cost := [2]int{3, 1}
	acc := 0
	for claw_machine in input {
		min_cost := 0
		for a in 0 ..< 100 {
			for b in 0 ..< 100 {
				if a * claw_machine.buttonA.x + b * claw_machine.buttonB.x ==
					   claw_machine.prize.x &&
				   a * claw_machine.buttonA.y + b * claw_machine.buttonB.y ==
					   claw_machine.prize.y {
					current_cost := a * cost[0] + b * cost[1]
					if min_cost == 0 {
						min_cost = current_cost
					}
					min_cost = min(min_cost, current_cost)
				}
			}
		}
		acc += min_cost
	}
	return fmt.aprintf("%v", acc)
}

part2_solve :: proc(input: InputT) -> OutputT {
	cost := [2]int{3, 1}
	acc := 0
	for claw_machine in input {
		lcm_x := math.lcm(claw_machine.buttonA.x, claw_machine.buttonA.y)
		button_a := [3]int {
			claw_machine.buttonA.x,
			claw_machine.buttonB.x,
			claw_machine.prize.x + 10000000000000,
		}
		button_b := [3]int {
			claw_machine.buttonA.y,
			claw_machine.buttonB.y,
			claw_machine.prize.y + 10000000000000,
		}

		norm_button_a := button_a * (lcm_x / button_a.x)
		norm_button_b := button_b * (lcm_x / button_b.x)
		diff := norm_button_a - norm_button_b

		assert(diff.x == 0)
		if diff.z % diff.y != 0 {
			continue
		}
		b_press := diff.z / diff.y
		if (button_a.z - b_press * button_a.y) % button_a.x != 0 {
			continue
		}
		a_press := (button_a.z - b_press * button_a.y) / button_a.x

		// fmt.println("a:", a_press, ", b:", b_press, button_a, button_b)
		assert(b_press >= 0 && a_press >= 0)

		acc += a_press * cost[0] + b_press * cost[1]
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

	input_test_1 := `Button A: X+94, Y+34
Button B: X+22, Y+67
Prize: X=8400, Y=5400

Button A: X+26, Y+66
Button B: X+67, Y+21
Prize: X=12748, Y=12176

Button A: X+17, Y+86
Button B: X+84, Y+37
Prize: X=7870, Y=6450

Button A: X+69, Y+23
Button B: X+27, Y+71
Prize: X=18641, Y=10279`

	if !run(&input_test_1, true, true, "480", "875318608908") {
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
