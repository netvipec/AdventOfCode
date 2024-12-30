use std::io::{self, BufRead};

type InputT = (Vec<usize>, Vec<u8>);
type Output1T = usize;
type Output2T = String;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let lines = stdin.lock().lines().next().unwrap().unwrap();
    let input1: Vec<usize> = lines
        .split(',')
        .map(|x| x.parse::<usize>().unwrap())
        .collect();
    let input2: Vec<u8> = lines.bytes().collect();
    return (input1, input2);
}

fn part1(input: &InputT) -> Output1T {
    let max_size = 256;
    let mut circular = vec![0; max_size];
    for i in 0..max_size {
        circular[i] = i;
    }
    let circular_size = circular.len();

    // println!("{:?}", circular);

    let mut current_pos = 0;
    let mut current_skip_size = 0;
    for length in &input.0 {
        let mut begin = current_pos;
        let mut end = current_pos + length - 1;
        while begin < end {
            let tmp = circular[begin % circular_size];
            circular[begin % circular_size] = circular[end % circular_size];
            circular[end % circular_size] = tmp;

            begin += 1;
            end -= 1;
        }

        // println!("{:?}", circular);

        current_pos = (current_pos + length + current_skip_size) % circular_size;
        current_skip_size += 1;
    }
    return circular[0] * circular[1];
}

fn part2(input: &InputT) -> Output2T {
    let add_input = vec![17, 31, 73, 47, 23];
    let mut new_input = vec![0; input.1.len() + add_input.len()];
    for i in 0..input.1.len() {
        new_input[i] = input.1[i] as usize;
    }
    for i in 0..add_input.len() {
        new_input[input.1.len() + i] = add_input[i];
    }

    // println!("{:?}", new_input);
    let max_size = 256;
    let mut circular = vec![0; max_size];
    for i in 0..max_size {
        circular[i] = i;
    }
    let circular_size = circular.len();

    // println!("{:?}", circular);

    let mut current_pos = 0;
    let mut current_skip_size = 0;
    for _round in 0..64 {
        for length in &new_input {
            let mut begin = current_pos;
            let mut end = current_pos + length - 1;
            while begin < end {
                let tmp = circular[begin % circular_size];
                circular[begin % circular_size] = circular[end % circular_size];
                circular[end % circular_size] = tmp;

                begin += 1;
                end -= 1;
            }

            // println!("{:?}", circular);

            current_pos = (current_pos + length + current_skip_size) % circular_size;
            current_skip_size += 1;
        }
    }

    let mut hash_str: String = "".to_string();
    let step = 16;
    for i in 0..circular_size / step {
        let mut hash = 0;
        for j in 0..step {
            hash ^= circular[i * step + j];
        }

        let p = format!("{:02x}", hash);
        hash_str.push_str(&p);
    }
    return hash_str;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
