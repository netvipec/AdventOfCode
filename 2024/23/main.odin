package main

import "core:fmt"
import "core:os"
import "core:slice"
import "core:strings"

InputT :: [dynamic][2]string
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	for line in strings.split_lines_iterator(content) {
		comp1, _, comp2 := strings.partition(line, "-")
		append(&input, [2]string{comp1, comp2})
	}

	return input
}

get_unique_list :: proc(computers: map[string]int) -> [dynamic]string {
	unique := [dynamic]string{}
	for computer in computers {
		append(&unique, computer)
	}
	return unique
}

get_connections :: proc(
	input: InputT,
) -> (
	map[[2]string]struct {},
	map[string]int,
) {
	connections := make(map[[2]string]struct {})
	computers := make(map[string]int)

	for connection in input {
		connections[connection] = {}
		connections[connection.yx] = {}
		if !(connection.x in computers) {
			computers[connection.x] = len(computers)
		}
		if !(connection.y in computers) {
			computers[connection.y] = len(computers)
		}
	}
	return connections, computers
}

part1_solve :: proc(input: InputT) -> OutputT {
	connections, computers := get_connections(input)
	defer delete(connections)
	defer delete(computers)

	computer_list := get_unique_list(computers)

	acc := 0
	for i in 0 ..< len(computer_list) {
		for j in i + 1 ..< len(computer_list) {
			for k in j + 1 ..< len(computer_list) {
				conn1 := [2]string{computer_list[i], computer_list[j]}
				conn2 := [2]string{computer_list[i], computer_list[k]}
				conn3 := [2]string{computer_list[j], computer_list[k]}
				if (strings.has_prefix(computer_list[i], "t") ||
					   strings.has_prefix(computer_list[j], "t") ||
					   strings.has_prefix(computer_list[k], "t")) &&
				   conn1 in connections &&
				   conn2 in connections &&
				   conn3 in connections {
					acc += 1
				}
			}
		}
	}

	return fmt.aprintf("%v", acc)
}

connected_to_group :: proc(
	group: map[string]struct {},
	connections: map[[2]string]struct {},
	candidate_computer: string,
) -> bool {
	for group_computer in group {
		connection := [2]string{candidate_computer, group_computer}
		if !(connection in connections) {
			return false}
	}
	return true
}

part2_solve :: proc(input: InputT) -> OutputT {
	connections, computers := get_connections(input)
	defer delete(connections)
	defer delete(computers)

	groups := make(map[int]map[string]struct {})
	for connection in input {
		group_index := len(groups)
		groups[group_index] = make(map[string]struct {})
		group_computers := &groups[group_index]
		group_computers[connection.x] = {}
		group_computers[connection.y] = {}
	}

	for c1 in computers {
		for _, &group in groups {
			if c1 in group {
				continue
			}

			if !connected_to_group(group, connections, c1) {
				continue
			}

			group[c1] = {}
		}
	}

	max_value := make(map[string]struct {})
	for _, group in groups {
		if len(group) > len(max_value) {
			max_value = group
		}
	}

	v := [dynamic]string{}
	for vv in max_value {
		append(&v, vv)
	}
	slice.sort(v[:])

	return fmt.aprintf("%v", strings.join(v[:], ","))
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

	input_test_1 := `kh-tc
qp-kh
de-cg
ka-co
yn-aq
qp-ub
cg-tb
vc-aq
tb-ka
wh-tc
yn-cg
kh-ub
ta-co
de-co
tc-td
tb-wq
wh-td
ta-ka
td-qp
aq-cg
wq-ub
ub-vc
de-ta
wq-aq
wq-vc
wh-yn
ka-de
kh-ta
co-tc
wh-qp
tb-vc
td-yn`

	if !run(&input_test_1, true, true, "7", "co,de,ka,ta") {
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
