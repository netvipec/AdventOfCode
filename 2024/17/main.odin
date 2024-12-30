package main

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"

InputT :: struct {
	registers: [dynamic]int,
	programs:  [dynamic]int,
}
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	regs, _, prog := strings.partition(content^, "\n\n")
	for reg in strings.split_lines_iterator(&regs) {
		append(&input.registers, strconv.atoi(reg[12:]))
	}
	progs := strings.split(prog[9:], ",")
	for p in progs {
		append(&input.programs, strconv.atoi(p))
	}

	return input
}

combo_operand :: proc(value: int, registers: [dynamic]int) -> int {
	if 0 <= value && value <= 3 {
		return value
	}
	if 4 <= value && value <= 6 {
		return registers[value - 4]
	}
	if value == 7 {
		assert(false)
	}
	assert(false)
	return -1
}

execute :: proc(
	instr: [dynamic]int,
	ip: ^int,
	registers: ^[dynamic]int,
	output: ^[dynamic]int,
) -> bool {
	if ip^ >= len(instr) {
		return true
	}
	switch instr[ip^] {
	case 0:
		//adv
		registers[0] /=
			1 << cast(uint)combo_operand(instr[ip^ + 1], registers^)
		ip^ += 2
	case 1:
		//bxl
		registers[1] ~= instr[ip^ + 1]
		ip^ += 2
	case 2:
		//bst
		registers[1] = combo_operand(instr[ip^ + 1], registers^) % 8
		ip^ += 2
	case 3:
		//jnz
		if registers[0] != 0 {
			ip^ = instr[ip^ + 1]
		} else {
			ip^ += 2
		}
	case 4:
		//bxc
		registers[1] ~= registers[2]
		ip^ += 2
	case 5:
		//out
		append(output, combo_operand(instr[ip^ + 1], registers^) % 8)
		ip^ += 2
	case 6:
		//bdv
		registers[1] =
			registers[0] /
			(1 << cast(uint)combo_operand(instr[ip^ + 1], registers^))
		ip^ += 2
	case 7:
		//cdv
		registers[2] =
			registers[0] /
			(1 << cast(uint)combo_operand(instr[ip^ + 1], registers^))
		ip^ += 2
	case:
		assert(false)
	}
	return false
}

part1_solve :: proc(input: InputT) -> OutputT {
	registers := [dynamic]int{}
	resize(&registers, len(input.registers))
	copy(registers[:], input.registers[:])
	output := [dynamic]int{}
	ip := 0
	for {
		// fmt.println("============", ip, registers, output)
		// if ip + 1 < len(input.programs) {
		// 	fmt.println(
		// 		"instr:",
		// 		input.programs[ip],
		// 		", oper:",
		// 		input.programs[ip + 1],
		// 	)
		// }
		stop := execute(input.programs, &ip, &registers, &output)
		if stop {
			break
		}
	}

	results := [dynamic]string{}
	for o in output {
		append(&results, fmt.aprintf("%v", o))
	}
	return strings.join(results[:], ",")
}

part2_solve :: proc(input: InputT) -> OutputT {
	registers := [dynamic]int{}
	resize(&registers, len(input.registers))
	output := [dynamic]int{}

	candidates := [dynamic][2]int{}
	append(&candidates, [2]int{0, 0})
	for len(candidates) > 0 {
		current := pop_front(&candidates)
		n := current.x
		offset := current.y

		// fmt.println("start:", offset)
		for a := 0; a < (1 << 10); a += 1 {
			copy(registers[:], input.registers[:])
			current_a := (a << cast(uint)(3 * offset)) | n
			registers[0] = current_a

			// fmt.println(registers)
			clear(&output)

			ip := 0
			output_index := 0
			for {
				stop := execute(input.programs, &ip, &registers, &output)
				if stop {
					if len(output) == len(input.programs) {
						return fmt.aprintf("%v", current_a)
					} else if len(output) > offset {
						// fmt.println(
						// 	"===>",
						// 	current_a,
						// 	n,
						// 	a,
						// 	output,
						// 	offset,
						// 	len(output),
						// )
						offset = len(output)
						mask := ((1 << cast(uint)(3 * offset)) - 1)
						n = current_a & mask
						append(&candidates, [2]int{n, offset})
					}
					break
				}
				if len(output) > len(input.programs) {
					break
				}
				if output_index < len(output) {
					if output[output_index] != input.programs[output_index] {
						break
					} else {
						output_index += 1
						// if output_index > offset {
						// 	fmt.println("--->", current_a, n, output)
						// }
						if output_index > offset + 1 {
							// fmt.println(
							// 	"~~~>",
							// 	current_a,
							// 	n,
							// 	a,
							// 	output,
							// 	offset,
							// 	output_index,
							// )
							offset += 1
							mask := ((1 << cast(uint)(3 * offset)) - 1)
							n = current_a & mask
							append(&candidates, [2]int{n, offset})
							break
						}
					}
				}
			}
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

	// 	input_test_01 := `Register A: 0
	// Register B: 0
	// Register C: 9

	// Program: 2,6`

	// 	if !run(&input_test_01, true, false, "-1", "-1") {
	// 		bad += 1
	// 	}

	// 	input_test_02 := `Register A: 0
	// Register B: 29
	// Register C: 0

	// Program: 1,7`

	// 	if !run(&input_test_02, true, false, "-1", "-1") {
	// 		bad += 1
	// 	}

	// 	input_test_03 := `Register A: 0
	// Register B: 2024
	// Register C: 43690

	// Program: 4,0`

	// 	if !run(&input_test_03, true, false, "-1", "-1") {
	// 		bad += 1
	// 	}

	input_test_2 := `Register A: 10
Register B: 0
Register C: 0

Program: 5,0,5,1,5,4`

	if !run(&input_test_2, true, false, "0,1,2", "-1") {
		bad += 1
	}

	input_test_3 := `Register A: 2024
Register B: 0
Register C: 0

Program: 0,1,5,4,3,0`

	if !run(&input_test_3, true, false, "4,2,5,6,7,7,7,7,3,1,0", "-1") {
		bad += 1
	}

	input_test_1 := `Register A: 729
Register B: 0
Register C: 0

Program: 0,1,5,4,3,0`

	if !run(&input_test_1, true, false, "4,6,3,5,6,3,5,2,1,0", "-1") {
		bad += 1
	}

	input_test_4 := `Register A: 117440
Register B: 0
Register C: 0

Program: 0,3,5,4,3,0`

	if !run(&input_test_4, true, false, "0,3,5,4,3,0", "-1") {
		bad += 1
	}

	// 	input_test_5 := `Register A: 2024
	// Register B: 0
	// Register C: 0

	// Program: 0,3,5,4,3,0`

	// 	if !run(&input_test_5, false, true, "-1", "117440") {
	// 		bad += 1
	// 	}

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
