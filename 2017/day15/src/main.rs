use std::io::{self, BufRead};

type InputT = Vec<usize>;
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| {
            let line = x.unwrap();
            line[24..].parse::<usize>().unwrap()
        })
        .collect();
    return input;
}

fn generate(pass: usize, factor: usize) -> usize {
    return (pass * factor) % 2147483647;
}

fn part1(input: &InputT) -> OutputT {
    let mut generator_a = input[0];
    let mut generator_b = input[1];
    let mut counter = 0;
    for _ in 0..40000000 {
        generator_a = generate(generator_a, 16807);
        generator_b = generate(generator_b, 48271);

        if (generator_a & 0xffff) == (generator_b & 0xffff) {
            counter += 1;
        }
    }
    return counter;
}

fn part2(input: &InputT) -> OutputT {
    let mut generator_a = input[0];
    let mut generator_b = input[1];
    let mut counter = 0;
    for _ in 0..5000000 {
        loop {
            generator_a = generate(generator_a, 16807);
            if generator_a % 4 == 0 {
                break;
            }
        }
        loop {
            generator_b = generate(generator_b, 48271);
            if generator_b % 8 == 0 {
                break;
            }
        }

        if (generator_a & 0xffff) == (generator_b & 0xffff) {
            counter += 1;
        }
    }
    return counter;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
