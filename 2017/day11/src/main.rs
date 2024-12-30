use std::io::{self, BufRead};

type InputT = Vec<String>;
type OutputT = i64;

struct HexagonalPosition {
    x: i64,
    y: i64,
    z: i64,
}

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .next()
        .unwrap()
        .unwrap()
        .split(',')
        .map(|x| x.to_string())
        .collect();
    return input;
}

fn move_pos(d: &str, position: &mut HexagonalPosition) {
    match d {
        "nw" => {
            position.x -= 1;
            position.y += 1;
        }
        "n" => {
            position.y += 1;
            position.z -= 1;
        }
        "ne" => {
            position.x += 1;
            position.z -= 1;
        }
        "sw" => {
            position.x -= 1;
            position.z += 1;
        }
        "s" => {
            position.y -= 1;
            position.z += 1;
        }
        "se" => {
            position.x += 1;
            position.y -= 1;
        }
        _ => panic!(),
    }
}

fn part1(input: &InputT) -> OutputT {
    let mut actual_position = HexagonalPosition { x: 0, y: 0, z: 0 };
    for d in input {
        move_pos(d, &mut actual_position);
    }
    return (actual_position.x.abs() + actual_position.y.abs() + actual_position.z.abs()) / 2;
}

fn part2(input: &InputT) -> OutputT {
    let mut actual_position = HexagonalPosition { x: 0, y: 0, z: 0 };
    let mut max_distance = 0;
    for d in input {
        move_pos(d, &mut actual_position);
        let distance =
            (actual_position.x.abs() + actual_position.y.abs() + actual_position.z.abs()) / 2;
        if distance > max_distance {
            max_distance = distance;
        }
    }
    return max_distance;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
