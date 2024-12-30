use std::collections::HashMap;
use std::io::{self, BufRead};

type InputT = i64;
type OutputT = i64;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .next()
        .unwrap()
        .unwrap()
        .parse()
        .unwrap();
    return input;
}

// 37 36  35  34  33  32 31
// 38 17  16  15  14  13 30
// 39 18   5   4   3  12 29
// 40 19   6   1   2  11 28
// 41 20   7   8   9  10 27
// 42 21  22  23  24  25 26
// 43 44  45  46  47  48 49

fn part1(input: InputT) -> OutputT {
    let mut n = 1;
    while n * n < input {
        n += 2;
    }

    let diff = n * n - input;
    // println!("n: {}, n*n: {}, n: {}, diff: {}", n, n * n, input, diff);

    let mut fixed_diff = diff;
    while fixed_diff >= n {
        fixed_diff -= n;
    }

    let half = n / 2;
    return (fixed_diff - half).abs() + half;
}

// 37 36  35  34  33  32 31
// 38 17  16  15  14  13 30
// 39 18   5   4   3  12 29
// 40 19   6   1   2  11 28
// 41 20   7   8   9  10 27
// 42 21  22  23  24  25 26
// 43 44  45  46  47  48 49

fn adjacent() -> Vec<(i64, i64)> {
    let mut res: Vec<(i64, i64)> = Vec::with_capacity(8);
    for x in -1..=1 {
        for y in -1..=1 {
            if x == 0 && y == 0 {
                continue;
            }
            res.push((x, y));
        }
    }
    return res;
}

fn part2(input: InputT) -> OutputT {
    let neighbours = adjacent();

    let mut v = HashMap::new();
    v.insert((0, 0), 1);
    let mut point = (0, 0);
    let mut layer = 1;
    let mut index = 1;
    let mut last_element = 0;
    while last_element < input {
        last_element = 0;

        let pos_in_layer = index - (layer * layer);
        if pos_in_layer == 0 {
            point.0 += 1;
        } else if pos_in_layer < layer + 1 {
            point.1 += 1;
        } else if pos_in_layer < 2 * (layer + 1) {
            point.0 -= 1;
        } else if pos_in_layer < 3 * (layer + 1) {
            point.1 -= 1;
        } else {
            point.0 += 1;
        }

        for n in neighbours.iter() {
            last_element += match v.get(&(point.0 + n.0, point.1 + n.1)) {
                Some(x) => *x,
                None => 0,
            };
        }

        println!("index: {}, value: {}", index, last_element);

        v.insert(point, last_element);
        index += 1;
        if index == (layer + 2) * (layer + 2) {
            layer += 2;
        }
    }
    return last_element;
}

fn main() {
    let input = read_input();

    let sol1 = part1(input);
    println!("Part1: {}", sol1);
    let sol2 = part2(input);
    println!("Part2: {}", sol2);
}
