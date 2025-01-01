package main

import "core:fmt"
import "core:math"
import "core:math/linalg"
import "core:os"
import "core:strconv"
import "core:strings"

Hailstone :: struct {
	position: [3]f64,
	velocity: [3]f64,
}

DistanceOrigin :: struct {
	distance: f64,
	index:    int,
}

TEST :: true

InputT :: [dynamic]Hailstone
OutputT :: int

lines_intercept :: proc(
	p1, p2: Hailstone,
	minimum, maximum: f64,
	part2: bool,
) -> bool {
	p1_pos := [2]f64{p1.position.x, p1.position.y}
	p1_vel := [2]f64{p1.velocity.x, p1.velocity.y}

	p2_pos := [2]f64{p2.position.x, p2.position.y}
	p2_vel := [2]f64{p2.velocity.x, p2.velocity.y}

	det1 := linalg.cross(p2_vel, p1_vel)
	det2 := linalg.cross(p1_vel, p2_vel)
	if det1 == 0 || det2 == 0 {
		return false
	}

	u :=
		(p1_vel.x * (p2_pos.y - p1_pos.y) + p1_vel.y * (p1_pos.x - p2_pos.x)) /
		det1
	t :=
		(p2_vel.x * (p1_pos.y - p2_pos.y) + p2_vel.y * (p2_pos.x - p1_pos.x)) /
		det2

	ip1 := p1_pos + p1_vel * [2]f64{t, t}
	ip2 := p2_pos + p2_vel * [2]f64{u, u}

	intercept :=
		(minimum <= ip1.x &&
			ip1.x < maximum &&
			minimum <= ip1.y &&
			ip1.y < maximum) &&
		((u >= 0 && t >= 0) || part2)

	if (part2 &&
		   (ip1.x != math.floor(ip1.x) ||
				   ip1.y != math.floor(ip1.y) ||
				   ip2.x != math.floor(ip2.x) ||
				   ip2.y != math.floor(ip2.y))) {
		intercept = false
	}

	// fmt.printfln(
	// 	"intercept, p1: %v\np2: %v\ndet1: %v, det2: %v, u: %v, t: %v\nip1: %v, ip2: %v",
	// 	p1,
	// 	p2,
	// 	det1,
	// 	det2,
	// 	u,
	// 	t,
	// 	ip1,
	// 	ip2,
	// )

	return intercept
}

part1_solve :: proc(input: InputT) -> OutputT {
	minimum := 7.0
	maximum := 27.0
	if !TEST {
		minimum = 200000000000000.0
		maximum = 400000000000000.0
	}

	intercept := 0
	for i in 0 ..< len(input) {
		for j in i + 1 ..< len(input) {
			if lines_intercept(input[i], input[j], minimum, maximum, false) {
				intercept += 1
			}
		}
	}

	return intercept
}

part2_solve :: proc(input: InputT) -> OutputT {
	p1 := input[0].position
	v1 := input[0].velocity
	p2 := input[1].position
	v2 := input[1].velocity

	fmt.println(p1, p2, v1, v2)

	t1 :=
		-linalg.dot(linalg.cross(p1, p2), v2) /
		linalg.dot(linalg.cross(v1, p2), v2)
	t2 :=
		-linalg.dot(linalg.cross(p1, p2), v1) /
		linalg.dot(linalg.cross(p1, v2), v1)

	c1 := p1 + t1 * v1
	c2 := p2 + t2 * v2
	v := (c2 - c1) / (t2 - t1)
	p := c1 - t1 * v
	fmt.println(t1, t2)
	fmt.println(c1, c2, v, p)

	return 0

	// closest := [dynamic]DistanceOrigin{}
	// for i in 0 ..< len(input) {
	// 	length := linalg.length2(input[i].position)
	// 	append(&closest, DistanceOrigin{length, i})
	// }

	// minimum_len := max(f64)
	// minimum_idx := -1
	// for i in 0 ..< len(closest) {
	// 	if closest[i].distance < minimum_len {
	// 		minimum_len = closest[i].distance
	// 		minimum_idx = closest[i].index
	// 	}
	// }

	// minimum_len2 := max(f64)
	// minimum_idx2 := -1
	// for i in 0 ..< len(closest) {
	// 	if i == minimum_idx {
	// 		continue
	// 	}
	// 	if closest[i].distance < minimum_len2 {
	// 		minimum_len2 = closest[i].distance
	// 		minimum_idx2 = closest[i].index
	// 	}
	// }

	// // fmt.println(minimum_len, minimum_idx, input[minimum_idx])
	// // fmt.println(minimum_len2, minimum_idx2, input[minimum_idx2])

	// max_idx := 20 if TEST else 500000

	// p1 := input[minimum_idx]
	// p2 := input[minimum_idx2]
	// for i in 0 ..= max_idx {
	// 	point1 := p1.position + [3]f64{f64(i), f64(i), f64(i)} * (p1.velocity)
	// 	for j in 0 ..= max_idx {
	// 		point2 :=
	// 			p2.position + [3]f64{f64(j), f64(j), f64(j)} * (p2.velocity)

	// 		vel := point2 - point1
	// 		gcd := math.gcd(int(vel.x), math.gcd(int(vel.y), int(vel.z)))
	// 		vel /= [3]f64{f64(gcd), f64(gcd), f64(gcd)}

	// 		bad := false
	// 		for idx in 0 ..< len(input) {
	// 			if idx == minimum_idx || idx == minimum_idx2 {
	// 				continue
	// 			}

	// 			if !lines_intercept(
	// 				input[idx],
	// 				Hailstone{point1, vel},
	// 				min(f64),
	// 				max(f64),
	// 				true,
	// 			) {
	// 				bad = true
	// 				break
	// 			}
	// 		}

	// 		if !bad {
	// 			fmt.println(point1 - vel * [3]f64{f64(i), f64(i), f64(i)}, vel)
	// 		}
	// 	}
	// }

	// return 0
}

read_input :: proc(content: ^string) -> InputT {
	hailstone := InputT{}
	for line in strings.split_lines_iterator(content) {
		begin, _, end := strings.partition(line, " @ ")
		bxyz := strings.split(begin, ",")
		exyz := strings.split(end, ",")
		assert(len(bxyz) == 3)
		assert(len(exyz) == 3)

		piece := Hailstone {
			{
				strconv.atof(strings.trim(bxyz[0], " ")),
				strconv.atof(strings.trim(bxyz[1], " ")),
				strconv.atof(strings.trim(bxyz[2], " ")),
			},
			{
				strconv.atof(strings.trim(exyz[0], " ")),
				strconv.atof(strings.trim(exyz[1], " ")),
				strconv.atof(strings.trim(exyz[2], " ")),
			},
		}
		append(&hailstone, piece)
	}
	return hailstone
}

main :: proc() {
	data :=
		os.read_entire_file("input.test" if TEST else "input") or_else os.exit(
			1,
		)
	defer delete(data)
	s := string(data)

	input := read_input(&s)
	// for p in input {
	// 	fmt.println(p)
	// }

	part1 := part1_solve(input)
	fmt.println("Part1:", part1)
	part2 := part2_solve(input)
	fmt.println("Part2:", part2)
}
