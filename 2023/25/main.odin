package main

import "core:fmt"
import "core:os"
import "core:strings"

ComponentConnections :: struct {
	component_index:  map[string]int,
	component_matrix: [dynamic][dynamic]int,
}

Best :: struct {
	minimum: int,
	list:    [dynamic]int,
}

TEST :: false

InputT :: ComponentConnections
OutputT :: int

max_array :: proc(arr: [dynamic]int) -> int {
	m := arr[0]
	mi := 0
	for i in 1 ..< len(arr) {
		if m < arr[i] {
			m = arr[i]
			mi = i
		}
	}
	return mi
}

part1_solve :: proc(input: InputT) -> OutputT {
	best := Best{max(int), {}}
	n := len(input.component_matrix)
	co := make([dynamic][dynamic]int, n)

	for i in 0 ..< n {
		co[i] = {i}
	}

	w := make([dynamic]int, n)
	for ph in 1 ..< n {
		copy(w[:], input.component_matrix[0][:])
		s := 0
		t := 0
		for _ in 0 ..< n - ph { 	// O(V^2) -> O(E log V) with prio. queue
			w[t] = min(int)
			s = t
			t = max_array(w)
			for i in 0 ..< n {
				w[i] += input.component_matrix[t][i]
			}
		}

		v := w[t] - input.component_matrix[t][t]
		if v < best.minimum {
			best.minimum = v
			best.list = co[t]
		}

		append(&co[s], ..co[t][:])
		for i in 0 ..< n {
			input.component_matrix[s][i] += input.component_matrix[t][i]
		}
		for i in 0 ..< n {
			input.component_matrix[i][s] = input.component_matrix[s][i]
		}
		input.component_matrix[0][t] = min(int)
	}

	return len(best.list) * (len(input.component_index) - len(best.list))
}

part2_solve :: proc(input: InputT) -> OutputT {
	return 0
}

read_input :: proc(content: ^string) -> InputT {
	connections := make(map[string][dynamic]string)
	defer delete(connections)

	input := ComponentConnections{}
	input.component_index = make(map[string]int)

	for line in strings.split_lines_iterator(content) {
		origin, _, destinations := strings.partition(line, ": ")
		assert(!(origin in connections))

		destinations_elem := strings.split(destinations, " ")
		connections[origin] = {}
		append(&connections[origin], ..destinations_elem[:])

		if !(origin in input.component_index) {
			input.component_index[origin] = len(input.component_index)
		}
		for component in destinations_elem {
			if !(component in input.component_index) {
				input.component_index[component] = len(input.component_index)
			}
		}
	}
	input.component_matrix = [dynamic][dynamic]int{}
	resize(&input.component_matrix, len(input.component_index))
	for i in 0 ..< len(input.component_matrix) {
		resize(&input.component_matrix[i], len(input.component_index))
	}
	for origin, destinations in connections {
		index_origin := input.component_index[origin]
		for destination in destinations {
			index_destination := input.component_index[destination]
			input.component_matrix[index_origin][index_destination] = 1
			input.component_matrix[index_destination][index_origin] = 1
		}
	}

	return input
}

main :: proc() {
	data :=
		os.read_entire_file("input.test" if TEST else "input") or_else os.exit(
			1,
		)
	defer delete(data)
	s := string(data)

	input := read_input(&s)
	// fmt.println(input.component_index)
	// fmt.println(input.component_matrix)

	part1 := part1_solve(input)
	fmt.println("Part1:", part1)
	part2 := part2_solve(input)
	fmt.println("Part2:", part2)
}
