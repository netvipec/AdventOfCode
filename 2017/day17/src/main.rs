use std::io::{self, BufRead};

type InputT = usize;
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .next()
        .unwrap()
        .unwrap()
        .parse::<usize>()
        .unwrap();
    return input;
}

fn spinlock_calc(input: &InputT, max_value: usize) -> (Vec<usize>, usize) {
    let mut circular = vec![0usize];

    let mut actual_pos = 0;
    while circular.len() <= max_value {
        let spinlock = input % circular.len();
        actual_pos = (actual_pos + spinlock) % circular.len();
        circular.insert(actual_pos + 1, circular.len());
        actual_pos += 1;
    }

    return (circular, actual_pos);
}

fn spinlock_calc2(input: &InputT, max_value: usize) -> usize {
    let mut circular_len = 1;
    let mut interest_pos = 0;
    let mut interest_value = 0;
    let mut actual_pos = 0;
    while circular_len <= max_value {
        let spinlock = input % circular_len;
        actual_pos = (actual_pos + spinlock) % circular_len;
        if actual_pos < interest_pos {
            interest_pos += 1;
        } else if actual_pos == interest_pos {
            interest_value = circular_len;
        }
        actual_pos += 1;
        circular_len += 1;
    }

    return interest_value;
}

fn part1(input: &InputT) -> OutputT {
    let (circular, last_pos) = spinlock_calc(input, 2017);

    return circular[(last_pos + 1) % circular.len()];
}

fn part2(input: &InputT) -> OutputT {
    let result = spinlock_calc2(input, 50000000);
    return result;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
