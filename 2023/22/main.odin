package main

import "core:fmt"
import "core:os"
import "core:slice"
import "core:strconv"
import "core:strings"

Piece :: struct {
	start: [3]int,
	end:   [3]int,
}

InputT :: [dynamic]Piece
OutputT :: int

less := proc(lhs, rhs: Piece) -> bool {
	if lhs.start.z == rhs.start.z {
		if lhs.start.y == rhs.start.y {
			return lhs.start.x < rhs.start.x
		}
		return lhs.start.y < rhs.start.y
	}
	return lhs.start.z < rhs.start.z
}

are_intervals_intersecting :: proc(a0, a1, b0, b1: int) -> bool {
	a0 := a0
	a1 := a1
	b0 := b0
	b1 := b1
	if (a1 < a0) {
		a1, a0 = a0, a1
	}

	if (b1 < b0) {
		b1, b0 = b0, b1
	}

	// 6 conditions:

	//  1)
	//         a0 ---------- a1                              a0 < b0 and a1 < b0
	//                              b0 ---------- b1         (no intersection)

	//  2)
	//                a0 ---------- a1
	//                       b0 ---------- b1                (intersection)

	//  3)
	//                a0 ------------------------ a1
	//                       b0 ---------- b1                (intersection)

	//  4)
	//                       a0 ---------- a1         
	//                b0 ------------------------ b1         (intersection)

	//  5)
	//                              a0 ---------- a1         (intersection)
	//                       b0 ---------- b1

	//  6)
	//                                     a0 ---------- a1  b0 < a0 and b1 < a0         
	//                b0 ---------- b1                       (no intersection)

	if b0 < a0 {
		// conditions 4, 5 and 6
		return a0 < b1 // conditions 4 and 5
	} else {
		// conditions 1, 2 and 3
		return b0 < a1 // conditions 2 and 3
	}
}

piece_intercept :: proc(lhs, rhs: Piece) -> bool {
	for i in 0 ..< len(lhs.start) {
		if !are_intervals_intersecting(
			lhs.start[i],
			lhs.end[i] + 1,
			rhs.start[i],
			rhs.end[i] + 1,
		) {
			return false
		}
	}
	return true
}

get_fall_piece :: proc(
	piece: Piece,
	new_well: [dynamic]Piece,
	avoid_idx: map[int]struct {},
) -> Piece {
	pp := piece
	p := pp
	for p.start.z > 1 {
		p.start.z -= 1
		p.end.z -= 1

		finish := false
		for idx in 0 ..< len(new_well) {
			if idx in avoid_idx {
				continue
			}
			intercep := piece_intercept(new_well[idx], p)
			if intercep {
				finish = true
				break
			}
		}
		if finish {
			p = pp
			break
		}
		pp = p
	}
	return p
}

print_well :: proc(new_well: [dynamic]Piece) {
	for p in new_well {
		fmt.println(p)
	}
}

part1_solve :: proc(well: InputT) -> OutputT {
	safely_desintegrated_brick := 0
	for i in 0 ..< len(well) {
		can_desintegrate := true
		for j in 0 ..< len(well) {
			if j == i {
				continue
			}

			piece := well[j]
			fall_piece := get_fall_piece(piece, well, {i = {}, j = {}})
			if fall_piece != piece {
				can_desintegrate = false
				break
			}
		}
		if can_desintegrate {
			safely_desintegrated_brick += 1
		}
	}

	return safely_desintegrated_brick
}

part2_solve :: proc(well: InputT) -> OutputT {
	safely_desintegrated_brick := 0
	for i in 0 ..< len(well) {
		fall_pieces := make(map[int]struct {})
		fall_pieces[i] = {}
		fall := 0

		for j in 0 ..< len(well) {
			if j == i {
				continue
			}

			piece := well[j]
			fall_pieces[j] = {}
			fall_piece := get_fall_piece(piece, well, fall_pieces)
			if fall_piece != piece {
				fall += 1
			} else {
				delete_key(&fall_pieces, j)
			}
		}
		safely_desintegrated_brick += fall
	}

	return safely_desintegrated_brick
}

read_input :: proc(content: ^string) -> InputT {
	pieces := InputT{}
	for line in strings.split_lines_iterator(content) {
		begin, _, end := strings.partition(line, "~")
		bxyz := strings.split(begin, ",")
		exyz := strings.split(end, ",")
		assert(len(bxyz) == 3)
		assert(len(exyz) == 3)

		piece := Piece {
			{
				strconv.atoi(bxyz[0]),
				strconv.atoi(bxyz[1]),
				strconv.atoi(bxyz[2]),
			},
			{
				strconv.atoi(exyz[0]),
				strconv.atoi(exyz[1]),
				strconv.atoi(exyz[2]),
			},
		}
		append(&pieces, piece)
	}
	return pieces
}

main :: proc() {
	data := os.read_entire_file("input") or_else os.exit(1)
	defer delete(data)
	s := string(data)

	input := read_input(&s)
	slice.sort_by(input[:], less)

	new_well := InputT{}
	for piece in input {
		p := get_fall_piece(
			piece,
			new_well,
			{len(new_well) = {}, len(new_well) = {}},
		)
		append(&new_well, p)
	}
	slice.sort_by(new_well[:], less)

	// for p in input {
	// 	fmt.println(p)
	// }

	part1 := part1_solve(new_well)
	fmt.println("Part1:", part1)
	part2 := part2_solve(new_well)
	fmt.println("Part2:", part2)
}
