package main

import "core:fmt"
import "core:os"
import "core:strings"

TEST :: false

Antenna :: struct {
	freq: rune,
	pos:  [2]int,
}

InputT :: struct {
	antennas: [dynamic]Antenna,
	size:     [2]int,
}
OutputT :: string

read_input :: proc(content: ^string) -> InputT {
	input := InputT{}

	r := 0
	C := 0
	for line in strings.split_lines_iterator(content) {
		for l, c in line {
			if ('0' <= l && l <= '9') ||
			   ('a' <= l && l <= 'z') ||
			   ('A' <= l && l <= 'Z') {
				append(&input.antennas, Antenna{l, [2]int{r, c}})
			}
		}
		C = len(line)
		r += 1
	}
	input.size = [2]int{r, C}

	return input
}

part1_solve :: proc(input: InputT) -> OutputT {
	antinodes := make(map[[2]int]struct {})
	for i in 0 ..< len(input.antennas) {
		for j in i + 1 ..< len(input.antennas) {
			antenna_i := input.antennas[i]
			antenna_j := input.antennas[j]
			if antenna_i.freq != antenna_j.freq {
				continue
			}
			d_ij := antenna_i.pos - antenna_j.pos
			d_ji := antenna_j.pos - antenna_i.pos

			new_li := antenna_i.pos + d_ij
			new_rj := antenna_j.pos + d_ji

			if 0 <= new_li.x &&
			   new_li.x < input.size.x &&
			   0 <= new_li.y &&
			   new_li.y < input.size.y {
				antinodes[new_li] = {}
			}
			if 0 <= new_rj.x &&
			   new_rj.x < input.size.x &&
			   0 <= new_rj.y &&
			   new_rj.y < input.size.y {
				antinodes[new_rj] = {}
			}
		}
	}
	return fmt.aprintf("%v", len(antinodes))
}

part2_solve :: proc(input: InputT) -> OutputT {
	antinodes := make(map[[2]int]struct {})

	antennas := make(map[rune]int)
	for antenna in input.antennas {
		antennas[antenna.freq] += 1
	}
	for freq, value in antennas {
		if value > 1 {
			for antenna in input.antennas {
				if antenna.freq == freq {
					antinodes[antenna.pos] = {}
				}
			}
		}
	}

	for i in 0 ..< len(input.antennas) {
		for j in i + 1 ..< len(input.antennas) {
			antenna_i := input.antennas[i]
			antenna_j := input.antennas[j]
			if antenna_i.freq != antenna_j.freq {
				continue
			}
			d_ij := antenna_i.pos - antenna_j.pos
			d_ji := antenna_j.pos - antenna_i.pos

			new_li := antenna_i.pos
			for {
				new_li += d_ij
				if 0 <= new_li.x &&
				   new_li.x < input.size.x &&
				   0 <= new_li.y &&
				   new_li.y < input.size.y {
					antinodes[new_li] = {}
				} else {
					break
				}
			}

			new_rj := antenna_j.pos
			for {
				new_rj += d_ji
				if 0 <= new_rj.x &&
				   new_rj.x < input.size.x &&
				   0 <= new_rj.y &&
				   new_rj.y < input.size.y {
					antinodes[new_rj] = {}
				} else {
					break
				}
			}
		}
	}
	return fmt.aprintf("%v", len(antinodes))
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
