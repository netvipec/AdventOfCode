use std::io::{self, BufRead};

type InputT = String;
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin.lock().lines().next().unwrap().unwrap();
    return input;
}

fn part1(input: &InputT) -> OutputT {
    let mut open_symbols = Vec::new();
    let mut cancel_next_symbol = false;
    let mut normal_groups = true;
    let mut total_groups = 0;
    for c in input.chars() {
        if cancel_next_symbol {
            cancel_next_symbol = false;
            continue;
        }
        if normal_groups {
            match c {
                '{' => open_symbols.push('}'),
                '}' => {
                    assert!(open_symbols.len() > 0 && *open_symbols.last().unwrap() == '}');
                    total_groups += open_symbols.len();
                    open_symbols.pop();
                }
                '!' => cancel_next_symbol = true,
                ',' => {}
                '<' => normal_groups = false,
                _ => panic!(),
            }
        } else {
            match c {
                '>' => normal_groups = true,
                '!' => cancel_next_symbol = true,
                _ => {}
            }
        }
    }

    return total_groups;
}

fn part2(input: &InputT) -> OutputT {
    let mut open_symbols = Vec::new();
    let mut cancel_next_symbol = false;
    let mut normal_groups = true;
    let mut total_non_canceled = 0;
    for c in input.chars() {
        if cancel_next_symbol {
            cancel_next_symbol = false;
            continue;
        }
        if normal_groups {
            match c {
                '{' => open_symbols.push('}'),
                '}' => {
                    assert!(open_symbols.len() > 0 && *open_symbols.last().unwrap() == '}');
                    open_symbols.pop();
                }
                '!' => cancel_next_symbol = true,
                ',' => {}
                '<' => normal_groups = false,
                _ => panic!(),
            }
        } else {
            match c {
                '>' => normal_groups = true,
                '!' => cancel_next_symbol = true,
                _ => total_non_canceled += 1,
            }
        }
    }

    return total_non_canceled;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
