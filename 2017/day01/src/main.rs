use std::io::BufRead;
use std::time::Instant;

type InputData = Vec<u8>;
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
        .flat_map(|l| l.bytes().map(|c| c - 0x30))
        .collect();
    return nums;
}

fn solve_part1(input: &InputData) -> Result1 {
    let sum = (0..input.len())
        .into_iter()
        .filter(|i| {
            return input[*i] == input[(*i + 1) % input.len()];
        })
        .map(|i| input[i] as Result1)
        .sum::<Result1>();
    return sum;
}

fn solve_part2(input: &InputData) -> Result2 {
    let halfway_around = input.len() / 2;
    let sum = (0..input.len())
        .into_iter()
        .filter(|i| {
            return input[*i] == input[(*i + halfway_around) % input.len()];
        })
        .map(|i| input[i] as Result1)
        .sum::<Result1>();
    return sum;
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
