package main

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:text/regex"

TEST :: false

InputT :: string
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	return content^
}

Parse :: enum {
	Init,
	FirstParam,
	SecondParam,
}

part1_solve :: proc(input: InputT) -> OutputT {
	rex, _ := regex.create("mul\\((\\d+),(\\d+)\\)", {regex.Flag.Global})
	defer regex.destroy(rex)

	si := 0
	acc := 0
	for {
		capture, _ := regex.match(rex, input[si:])
		defer regex.destroy(capture)
		if len(capture.groups) == 0 {
			break
		}
		si += capture.pos[0].y
		fn := strconv.atoi(capture.groups[1])
		sn := strconv.atoi(capture.groups[2])
		acc += fn * sn
	}

	return fmt.aprintf("%v", acc)
}

part2_solve :: proc(input: InputT) -> OutputT {
	phase := Parse.Init
	i := 0
	acc := 0
	fp := 0
	fn := 0
	sp := 0
	mult_enable := true
	for {
		if i >= len(input) {
			break
		}
		if i + 4 <= len(input) {
			cmd := input[i:i + 4]
			if cmd == "do()" {
				mult_enable = true
			}
		}
		if i + 7 <= len(input) {
			cmd := input[i:i + 7]
			if cmd == "don't()" {
				mult_enable = false
			}
		}

		if mult_enable {
			switch phase {
			case .Init:
				if i + 4 >= len(input) {
					break
				}
				cmd := input[i:i + 4]
				if i + 3 < len(input) && cmd == "mul(" {
					phase = .FirstParam
					i += 4
					fp = i
				}
			case .FirstParam:
				if '0' <= input[i] && input[i] <= '9' {
					break
				} else if input[i] == ',' {
					fn = strconv.atoi(input[fp:i])
					sp = i + 1
					phase = .SecondParam
				} else {
					phase = .Init
				}
			case .SecondParam:
				if '0' <= input[i] && input[i] <= '9' {
					break
				} else if input[i] == ')' {
					sn := strconv.atoi(input[sp:i])
					acc += fn * sn
					phase = .Init
				} else {
					phase = .Init
				}
			}
		}
		i += 1
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
