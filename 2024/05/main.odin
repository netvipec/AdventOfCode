package main

import "core:fmt"
import "core:os"
import "core:slice"
import "core:strconv"
import "core:strings"

TEST :: false

InputData :: struct {
	ordering: [dynamic][2]int,
	updates:  [dynamic][dynamic]int,
}

InputT :: InputData
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	first, _, second := strings.partition(content^, "\n\n")
	for line in strings.split_lines_iterator(&first) {
		before, _, after := strings.partition(line, "|")

		append(
			&input.ordering,
			[2]int{strconv.atoi(before), strconv.atoi(after)},
		)
	}
	for line in strings.split_lines_iterator(&second) {
		updates := strings.split(line, ",")

		u := [dynamic]int{}
		for update in updates {
			append(&u, strconv.atoi(update))
		}

		append(&input.updates, u)
	}

	return input
}

correctly_order :: proc(update: [dynamic]int, order: [dynamic]int) -> bool {
	last := -1
	for upd in update {
		index, _ := slice.linear_search(order[:], upd)
		if index < last {
			return false
		}
		last = index
	}
	return true
}

part1_solve :: proc(input: InputT) -> OutputT {
	acc := 0
	for upd in input.updates {
		ok := true
		outer: for i in 0 ..< len(upd) {
			for j in i + 1 ..< len(upd) {
				_, found := slice.linear_search(
					input.ordering[:],
					[2]int{upd[j], upd[i]},
				)
				if found {
					ok = false
					break outer
				}
			}

		}
		if ok {
			acc += upd[len(upd) / 2]
		}
	}

	return fmt.aprintf("%v", acc)
}

part2_solve :: proc(input: InputT) -> OutputT {
	input := input

	acc := 0
	for upd in input.updates {
		ok := false
		modified := true
		for modified {
			modified = false
			outer: for i in 0 ..< len(upd) {
				for j in i + 1 ..< len(upd) {
					_, found := slice.linear_search(
						input.ordering[:],
						[2]int{upd[j], upd[i]},
					)
					if found {
						modified = true
						tmp := upd[j]
						upd[j] = upd[i]
						upd[i] = tmp
						ok = true
						break outer
					}
				}

			}
		}
		if ok {
			acc += upd[len(upd) / 2]
		}
	}

	return fmt.aprintf("%v", acc)
}

main :: proc() {
	data :=
		os.read_entire_file("input.test" if TEST else "input") or_else os.exit(
			1,
		)
	defer delete(data)
	s := string(data)

	input := read_input(&s)
	// fmt.println(input)

	part1 := part1_solve(input)
	fmt.println("Part1:", part1)
	part2 := part2_solve(input)
	fmt.println("Part2:", part2)
}
