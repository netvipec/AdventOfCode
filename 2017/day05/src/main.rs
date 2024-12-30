use std::io::{self, BufRead};

type InputT = Vec<i64>;
type OutputT = i64;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| x.unwrap().parse::<i64>().unwrap())
        .collect();
    return input;
}

fn part1(input: &InputT) -> OutputT {
    let mut executed_instructions = 0;
    let mut ip = 0i64;
    let mut mem = input.clone();
    while 0 <= ip && ip < mem.len() as i64 {
        let v = mem[ip as usize];
        mem[ip as usize] += 1;
        ip += v;
        executed_instructions += 1;
    }
    return executed_instructions;
}

fn part2(input: &InputT) -> OutputT {
    let mut executed_instructions = 0;
    let mut ip = 0i64;
    let mut mem = input.clone();
    while 0 <= ip && ip < mem.len() as i64 {
        let v = mem[ip as usize];
        if v >= 3 {
            mem[ip as usize] -= 1;
        } else {
            mem[ip as usize] += 1;
        }
        ip += v;
        executed_instructions += 1;
    }
    return executed_instructions;
}

fn main() {
    let input = read_input();

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
