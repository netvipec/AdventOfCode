package main

import "core:fmt"
import "core:os"
import "core:slice"
import "core:strconv"
import "core:strings"

Equation :: struct {
	operand1:  string,
	operand2:  string,
	operation: string,
	output:    string,
}
Equations :: [dynamic]Equation
InputT :: struct {
	variables: map[string]bool,
	equations: Equations,
}
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	variables, _, equations := strings.partition(content^, "\n\n")
	for line in strings.split_lines_iterator(&variables) {
		variable_name, _, variable_value := strings.partition(line, ": ")
		input.variables[variable_name] = variable_value == "1"
	}

	for line in strings.split_lines_iterator(&equations) {
		equation, _, destination := strings.partition(line, " -> ")
		equation_operands := strings.split(equation, " ")
		// fmt.println(equation_operands)
		assert(len(equation_operands) == 3)
		append(
			&input.equations,
			Equation {
				equation_operands[0],
				equation_operands[2],
				equation_operands[1],
				destination,
			},
		)
	}
	// fmt.println(input.equations)

	return input
}

equation_ready :: proc(
	operands: [2]string,
	variables: ^map[string]bool,
	output: ^map[string]bool,
) -> bool {
	return(
		(operands[0] in variables || operands[0] in output) &&
		(operands[1] in variables || operands[1] in output) \
	)
}

get_variable_value :: proc(
	variable_name: string,
	variables: ^map[string]bool,
	output: ^map[string]bool,
) -> bool {
	if variable_name in output {
		return output[variable_name]
	}
	assert(variable_name in variables)
	return variables[variable_name]
}

finished :: proc(
	output: ^map[string]bool,
	variable_list: ^[dynamic]string,
) -> bool {
	for variable in variable_list {
		if !(variable in output) {
			return false
		}
	}
	return true
}

execute_system :: proc(
	equations: ^Equations,
	variables: ^map[string]bool,
	variables_list: ^[dynamic]string,
) -> (
	bool,
	map[string]bool,
) {
	output := make(map[string]bool)
	seen := make(map[string]struct {})
	defer delete(seen)

	for {
		executed := 0
		for equation in equations {
			if equation.output in seen {
				continue
			}

			// fmt.println(variables, output, equation)
			if !equation_ready(
				[2]string{equation.operand1, equation.operand2},
				variables,
				&output,
			) {
				continue
			}

			executed += 1

			lhs := get_variable_value(equation.operand1, variables, &output)
			rhs := get_variable_value(equation.operand2, variables, &output)
			// fmt.println(equation_operands)
			if equation.operation == "AND" {
				output[equation.output] = lhs & rhs
			} else if equation.operation == "OR" {
				output[equation.output] = lhs | rhs
			} else if equation.operation == "XOR" {
				output[equation.output] = lhs ~ rhs
			} else {
				assert(false)
			}

			// fmt.println(equation, output)

			seen[equation.output] = {}
		}

		if finished(&output, variables_list) {
			break
		}

		if executed == 0 {
			return false, {}
		}
	}

	return true, output
}

get_variables_with_name :: proc(
	equations: ^[dynamic]Equation,
) -> [dynamic]string {
	variables_list := [dynamic]string{}
	for equation in equations {
		if strings.has_prefix(equation.output, "z") {
			append(&variables_list, equation.output)
		}
	}
	slice.reverse_sort(variables_list[:])
	return variables_list
}

part1_solve :: proc(input: ^InputT) -> OutputT {
	z_variables := get_variables_with_name(&input.equations)
	// fmt.println(variables_list)

	terminate, output := execute_system(
		&input.equations,
		&input.variables,
		&z_variables,
	)
	defer delete(output)
	assert(terminate)

	n := 0
	for variable in z_variables {
		n = (n << 1) | cast(int)output[variable]
	}
	return fmt.aprintf("%v", n)
}

BfsData :: struct {
	variable_name: string,
	length:        int,
}

get_variable_name :: proc(bit_index: int, base: string) -> string {
	buf: [4]byte
	assert(bit_index >= 0)
	if bit_index < 10 {
		return strings.concatenate(
			[]string{base, "0", strconv.itoa(buf[:], bit_index)},
		)
	}
	return strings.concatenate([]string{base, strconv.itoa(buf[:], bit_index)})
}

get_variable_data :: proc(
	equations: ^Equations,
	output_variable: string,
	max_depth: int,
) -> map[string]struct {} {
	process := [dynamic]BfsData{}
	seen := make(map[string]struct {})
	defer delete(seen)

	dependencies := make(map[string]struct {})

	append(&process, BfsData{output_variable, 0})

	for len(process) > 0 {
		current := pop_front(&process)
		if current.variable_name in seen {
			continue
		}
		if current.length > max_depth {
			continue
		}
		seen[current.variable_name] = {}

		for equation in equations {
			if equation.output == current.variable_name {
				append(
					&process,
					BfsData{equation.operand1, current.length + 1},
					BfsData{equation.operand2, current.length + 1},
				)
				dependencies[equation.operand1] = {}
				dependencies[equation.operand2] = {}
				break
			}
		}
	}
	return dependencies
}

TRIES :: [][3][2]bool {
	{{false, false}, {false, false}, {false, false}},
	{{true, false}, {false, false}, {true, false}},
	{{false, false}, {true, false}, {true, false}},
	{{true, false}, {true, false}, {false, true}},
}

init_variables :: proc(output_variables: ^[dynamic]string) -> map[string]bool {
	variables := make(map[string]bool)
	for variable_name in output_variables {
		variables[variable_name] = false
	}
	return variables
}

get_wrong_output :: proc(output: ^map[string]bool) -> [dynamic]string {
	z := [dynamic]string{}
	for o, v in output {
		if v {
			append(&z, o)
		}
	}
	return z
}

check_validity :: proc(
	equations: ^Equations,
	input_variables: ^[dynamic]string,
	output_variables: ^[dynamic]string,
	z_variables: ^[dynamic]string,
	max_bit_index: int,
) -> (
	int,
	[dynamic]string,
) {
	variables := init_variables(input_variables)

	for bit_index in 0 ..= max_bit_index {
		for trie in TRIES {
			x_var_name := get_variable_name(bit_index, "x")
			y_var_name := get_variable_name(bit_index, "y")
			variables[x_var_name] = trie[0].x
			variables[y_var_name] = trie[1].x

			defer {
				variables[x_var_name] = false
				variables[y_var_name] = false
			}

			_, output := execute_system(equations, &variables, z_variables)
			defer delete(output)

			z_var_name := get_variable_name(bit_index, "z")
			z_var_name_next := get_variable_name(bit_index + 1, "z")
			if output[z_var_name] != trie[2].x ||
			   output[z_var_name_next] != trie[2].y {
				return bit_index, get_wrong_output(&output)
			}
			for bit in 0 ..= max_bit_index {
				if bit_index == bit || bit_index + 1 == bit {
					continue
				}
				z_var := get_variable_name(bit, "z")
				if output[z_var] {
					return bit_index, get_wrong_output(&output)
				}
			}
		}
	}
	return max_bit_index + 1, {}
}

get_possibilities :: proc(
	equations: ^Equations,
	target_variables: ^[dynamic]string,
	max_depth: int,
) -> [dynamic]string {
	target_variables_dependencies := make(map[string]struct {})
	defer delete(target_variables_dependencies)

	process := [dynamic]BfsData{}
	seen := make(map[string]struct {})
	defer delete(seen)
	for target_variable in target_variables {
		append(&process, BfsData{target_variable, 0})

		for len(process) > 0 {
			current := pop_front(&process)
			if current.variable_name in seen {
				continue
			}
			if current.length >= max_depth {
				continue
			}
			seen[current.variable_name] = {}

			for equation in equations {
				if equation.output == current.variable_name {
					// fmt.println(
					// 	z_variable,
					// 	equation.operand1,
					// 	equation.operand2,
					// )
					append(
						&process,
						BfsData{equation.operand1, current.length + 1},
						BfsData{equation.operand2, current.length + 1},
					)
					if !strings.has_prefix(equation.operand1, "x") &&
					   !strings.has_prefix(equation.operand1, "y") {
						target_variables_dependencies[equation.operand1] = {}
					}
					if !strings.has_prefix(equation.operand2, "x") &&
					   !strings.has_prefix(equation.operand2, "y") {
						target_variables_dependencies[equation.operand2] = {}
					}
					break
				}
			}
		}

		// fmt.println(z_variable, z_variables_dependencies[z_variable])
	}
	target_variables_dependencies[slice.first(target_variables[:])] = {}
	target_variables_dependencies[slice.last(target_variables[:])] = {}

	possibilities := [dynamic]string{}
	for p in target_variables_dependencies {
		append(&possibilities, p)
	}
	return possibilities
}

solve :: proc(
	max_x: int,
	output_variables: ^[dynamic]string,
	z_variables: ^[dynamic]string,
	equations: ^Equations,
	input_variables: ^[dynamic]string,
	variables: ^map[string]bool,
	swaps: ^map[string]string,
) -> (
	bool,
	map[string]string,
) {
	first_bad_bit, active_bits := check_validity(
		equations,
		input_variables,
		output_variables,
		z_variables,
		max_x,
	)

	if first_bad_bit > max_x {
		return true, swaps^
	}

	if len(swaps) >= 8 {
		return false, {}
	}

	new_equations := Equations{}
	resize(&new_equations, len(equations))
	copy(new_equations[:], equations[:])

	targets := [dynamic]string {
		get_variable_name(first_bad_bit, "z"),
		get_variable_name(first_bad_bit + 1, "z"),
	}
	possibilities := get_possibilities(equations, &targets, 1)

	// fmt.println(first_bad_bit, swaps, possibilities, active_bits)

	for ii in possibilities {
		i, _ := slice.linear_search(output_variables[:], ii)
		var1 := output_variables[i]
		if var1 in swaps {
			return false, {}
		}

		for jj in active_bits {
			if ii == jj {
				continue
			}
			j, _ := slice.linear_search(output_variables[:], jj)
			var2 := output_variables[j]
			if var2 in swaps {
				continue
			}

			new_equations[i].output = output_variables[j]
			new_equations[j].output = output_variables[i]

			defer {
				new_equations[i].output = output_variables[i]
				new_equations[j].output = output_variables[j]
			}

			new_first_bad_bit, _ := check_validity(
				&new_equations,
				input_variables,
				output_variables,
				z_variables,
				max_x,
			)

			if new_first_bad_bit < first_bad_bit {
				continue
			}

			new_swaps := make(map[string]string)
			defer delete(new_swaps)

			for k, v in swaps {
				new_swaps[k] = v
			}
			new_swaps[var1] = var2
			new_swaps[var2] = var1

			solved, result := solve(
				max_x,
				output_variables,
				z_variables,
				&new_equations,
				input_variables,
				variables,
				&new_swaps,
			)
			if solved {
				return true, result
			}
		}
	}
	return false, {}
}

part2_solve :: proc(input: ^InputT) -> OutputT {
	output_variables := [dynamic]string{}
	for equation in input.equations {
		append(&output_variables, equation.output)
	}

	max_x := 0
	for variable_name in input.variables {
		if strings.has_prefix(variable_name, "x") {
			max_x = max(max_x, strconv.atoi(variable_name[1:]))
		}
	}

	variables := [dynamic]string{}
	for v in input.variables {
		append(&variables, v)
	}
	z_variables := get_variables_with_name(&input.equations)

	swaps := make(map[string]string)
	defer delete(swaps)

	_, final_swaps := solve(
		max_x,
		&output_variables,
		&z_variables,
		&input.equations,
		&variables,
		&input.variables,
		&swaps,
	)

	solution := [dynamic]string{}
	defer delete(solution)
	for fs in final_swaps {
		append(&solution, fs)
	}
	slice.sort(solution[:])

	return fmt.aprintf("%v", strings.join(solution[:], ","))
}

run :: proc(
	content: ^string,
	part1, part2: bool,
	expected_part1, expected_part2: string,
) -> bool {
	input := read_input(content)
	// fmt.println(input)

	if part1 {
		part1_solution := part1_solve(&input)
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
		part2_solution := part2_solve(&input)
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

	input_test_1 := `x00: 1
x01: 1
x02: 1
y00: 0
y01: 1
y02: 0

x00 AND y00 -> z00
x01 XOR y01 -> z01
x02 OR y02 -> z02`

	if !run(&input_test_1, true, false, "4", "-1") {
		bad += 1
	}

	input_test_2 := `x00: 1
x01: 0
x02: 1
x03: 1
x04: 0
y00: 1
y01: 1
y02: 1
y03: 1
y04: 1

ntg XOR fgs -> mjb
y02 OR x01 -> tnw
kwq OR kpj -> z05
x00 OR x03 -> fst
tgd XOR rvg -> z01
vdt OR tnw -> bfw
bfw AND frj -> z10
ffh OR nrd -> bqk
y00 AND y03 -> djm
y03 OR y00 -> psh
bqk OR frj -> z08
tnw OR fst -> frj
gnj AND tgd -> z11
bfw XOR mjb -> z00
x03 OR x00 -> vdt
gnj AND wpb -> z02
x04 AND y00 -> kjc
djm OR pbm -> qhw
nrd AND vdt -> hwm
kjc AND fst -> rvg
y04 OR y02 -> fgs
y01 AND x02 -> pbm
ntg OR kjc -> kwq
psh XOR fgs -> tgd
qhw XOR tgd -> z09
pbm OR djm -> kpj
x03 XOR y03 -> ffh
x00 XOR y04 -> ntg
bfw OR bqk -> z06
nrd XOR fgs -> wpb
frj XOR qhw -> z04
bqk OR frj -> z07
y03 OR x01 -> nrd
hwm AND bqk -> z03
tgd XOR rvg -> z12
tnw OR pbm -> gnj`

	if !run(&input_test_2, true, false, "2024", "-1") {
		bad += 1
	}

	input_test_3 := `x00: 0
x01: 1
x02: 0
x03: 1
x04: 0
x05: 1
y00: 0
y01: 0
y02: 1
y03: 1
y04: 0
y05: 1

x00 AND y00 -> z05
x01 AND y01 -> z02
x02 AND y02 -> z01
x03 AND y03 -> z03
x04 AND y04 -> z04
x05 AND y05 -> z00`

	if !run(&input_test_3, false, false, "-1", "z00,z01,z02,z05") {
		bad += 1
	}

	return bad
}

main :: proc() {
	fail_tests := run_tests()
	if fail_tests != 0 {
		fmt.println("Some tests fail:", fail_tests)
		return
	} else {
		fmt.println("All tests pass.")
	}

	data := os.read_entire_file("input") or_else os.exit(1)
	defer delete(data)
	s := string(data)

	input := read_input(&s)
	// fmt.println(input)

	fmt.println("Executing Real Part1...")
	part1 := part1_solve(&input)
	fmt.println("Part1:", part1)
	fmt.println("Executing Real Part2...")
	part2 := part2_solve(&input)
	fmt.println("Part2:", part2)
}
