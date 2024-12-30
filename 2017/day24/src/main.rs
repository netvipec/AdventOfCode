use std::collections::HashMap;
use std::io::{self, BufRead};

type InputT = Vec<(usize, usize)>;
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| {
            let line = x.unwrap();
            let elements = line.split('/').collect::<Vec<&str>>();
            let first = elements[0].parse::<usize>().unwrap();
            let second = elements[1].parse::<usize>().unwrap();
            if first < second {
                (first, second)
            } else {
                (second, first)
            }
        })
        .collect();
    return input;
}

fn solve1(
    input: &InputT,
    map_index: &HashMap<usize, Vec<usize>>,
    origin: usize,
    used_bridges: &mut Vec<bool>,
    strength: usize,
) -> usize {
    match map_index.get(&origin) {
        Some(x) => {
            let mut max_s = strength;
            x.iter().for_each(|e| {
                if !used_bridges[*e] {
                    let other = if input[*e].0 == origin {
                        input[*e].1
                    } else {
                        input[*e].0
                    };
                    used_bridges[*e] = true;
                    let s = solve1(
                        &input,
                        &map_index,
                        other,
                        used_bridges,
                        strength + input[*e].0 + input[*e].1,
                    );
                    if s > max_s {
                        max_s = s;
                    }
                    used_bridges[*e] = false;
                }
            });
            return max_s;
        }
        None => return strength,
    };
}

fn part1(input: &InputT) -> OutputT {
    let mut map_index: HashMap<usize, Vec<usize>> = HashMap::new();
    input.iter().enumerate().for_each(|ie| {
        map_index.entry((ie.1).0).or_insert(Vec::new()).push(ie.0);
        if (ie.1).0 != (ie.1).1 {
            map_index.entry((ie.1).1).or_insert(Vec::new()).push(ie.0);
        }
    });

    let mut used_bridges = vec![false; input.len()];
    return solve1(&input, &map_index, 0, &mut used_bridges, 0);
}

fn solve2(
    input: &InputT,
    map_index: &HashMap<usize, Vec<usize>>,
    origin: usize,
    used_bridges: &mut Vec<bool>,
    length: usize,
    strength: usize,
) -> (usize, usize) {
    match map_index.get(&origin) {
        Some(x) => {
            let mut max_l = length;
            let mut max_s = strength;
            x.iter().for_each(|e| {
                if !used_bridges[*e] {
                    let other = if input[*e].0 == origin {
                        input[*e].1
                    } else {
                        input[*e].0
                    };
                    used_bridges[*e] = true;
                    let s = solve2(
                        &input,
                        &map_index,
                        other,
                        used_bridges,
                        length + 1,
                        strength + input[*e].0 + input[*e].1,
                    );
                    if s.0 > max_l {
                        max_l = s.0;
                        max_s = s.1;
                    } else if s.0 == max_l && s.1 > max_s {
                        max_s = s.1;
                    }
                    used_bridges[*e] = false;
                }
            });
            return (max_l, max_s);
        }
        None => return (length, strength),
    };
}

fn part2(input: &InputT) -> OutputT {
    let mut map_index: HashMap<usize, Vec<usize>> = HashMap::new();
    input.iter().enumerate().for_each(|ie| {
        map_index.entry((ie.1).0).or_insert(Vec::new()).push(ie.0);
        if (ie.1).0 != (ie.1).1 {
            map_index.entry((ie.1).1).or_insert(Vec::new()).push(ie.0);
        }
    });

    let mut used_bridges = vec![false; input.len()];
    return solve2(&input, &map_index, 0, &mut used_bridges, 0, 0).1;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
