extern crate itertools;

use itertools::Itertools;
use std::io::BufRead;
use std::time::Instant;

type InputData = Vec<Vec<i64>>;
type Result1 = i64;
type Result2 = i64;

fn read_input() -> InputData {
    let lines: Vec<String> = std::io::stdin()
        .lock()
        .lines()
        .map(|line| line.unwrap())
        .collect();
    let nums: InputData = lines
        .iter()
        .map(|l| l.split("\t").map(|n| n.parse::<i64>().unwrap()).collect())
        .collect();

    // println!("{:?}", nums);
    return nums;
}

fn solve_part1(input: &InputData) -> Result1 {
    let sol = input
        .iter()
        .map(|vn| vn.iter().max().unwrap() - vn.iter().min().unwrap())
        .sum();
    return sol;
}

fn solve_part2(input: &InputData) -> Result2 {
    let sol = input
        .iter()
        .map(|vn| {
            let d = vn
                .iter()
                .combinations(2)
                .find(|comb| {
                    return (comb[0] > comb[1] && comb[0] % comb[1] == 0)
                        || (comb[1] > comb[0] && comb[1] % comb[0] == 0);
                })
                .unwrap();
            if d[0] > d[1] {
                d[0] / d[1]
            } else {
                d[1] / d[0]
            }
        })
        .sum();
    return sol;
}

fn main() {
    let input_data = read_input();

    let now = Instant::now();

    {
        let res1 = solve_part1(&input_data);
        println!("Part1: {}", res1);

        let res2 = solve_part2(&input_data);
        println!("Part2: {}", res2);
    }

    let elapsed = now.elapsed();
    println!("Elapsed: {:?}", elapsed);
}
