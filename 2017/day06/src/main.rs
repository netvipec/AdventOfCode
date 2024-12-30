use std::collections::{HashMap, HashSet};
use std::io::{self, BufRead};

type InputT = Vec<i64>;
type OutputT = i64;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .next()
        .unwrap()
        .unwrap()
        .split('\t')
        .map(|x| x.parse::<i64>().unwrap())
        .collect();
    return input;
}

fn part1(input: &InputT) -> OutputT {
    let mut input_mod = input.clone();
    let input_size = input_mod.len() as i64;
    let mut previous_states = HashSet::new();
    let mut counter = 0;
    while !previous_states.contains(&input_mod) {
        previous_states.insert(input_mod.clone());
        counter += 1;

        let to_increase;
        let rest_to_increase;
        let pos;
        {
            let mut iter = input_mod.iter().enumerate();
            let init = iter.next().unwrap();
            let max_iter = iter
                .try_fold(init, |acc, x| {
                    // return None if x is NaN
                    let cmp = x.1.partial_cmp(acc.1)?;
                    // if x is greater the acc
                    let max = if let std::cmp::Ordering::Greater = cmp {
                        x
                    } else {
                        acc
                    };
                    Some(max)
                })
                .unwrap();

            to_increase = max_iter.1 / input_size;
            rest_to_increase = max_iter.1 % input_size;
            pos = max_iter.0;
        }
        input_mod[pos] = 0;
        if to_increase > 0 {
            for i in input_mod.iter_mut() {
                *i += to_increase;
            }
        }
        if rest_to_increase > 0 {
            if pos + 1 + (rest_to_increase as usize) >= input_size as usize {
                for i in input_mod.iter_mut().skip(pos + 1) {
                    *i += 1;
                }
                let until = pos + 1 + (rest_to_increase as usize) - input_size as usize;
                for i in input_mod.iter_mut().take(until) {
                    *i += 1;
                }
            } else {
                for i in input_mod
                    .iter_mut()
                    .skip(pos + 1)
                    .take(rest_to_increase as usize)
                {
                    *i += 1;
                }
            }
        }
        // println!("{:?}", input_mod);
    }
    return counter;
}

fn part2(input: &InputT) -> OutputT {
    let mut input_mod = input.clone();
    let input_size = input_mod.len() as i64;
    let mut previous_states = HashMap::new();
    let mut counter = 0;
    while !previous_states.contains_key(&input_mod) {
        previous_states.insert(input_mod.clone(), counter);
        counter += 1;

        let to_increase;
        let rest_to_increase;
        let pos;
        {
            let mut iter = input_mod.iter().enumerate();
            let init = iter.next().unwrap();
            let max_iter = iter
                .try_fold(init, |acc, x| {
                    // return None if x is NaN
                    let cmp = x.1.partial_cmp(acc.1)?;
                    // if x is greater the acc
                    let max = if let std::cmp::Ordering::Greater = cmp {
                        x
                    } else {
                        acc
                    };
                    Some(max)
                })
                .unwrap();

            to_increase = max_iter.1 / input_size;
            rest_to_increase = max_iter.1 % input_size;
            pos = max_iter.0;
        }
        input_mod[pos] = 0;
        if to_increase > 0 {
            for i in input_mod.iter_mut() {
                *i += to_increase;
            }
        }
        if rest_to_increase > 0 {
            if pos + 1 + (rest_to_increase as usize) >= input_size as usize {
                for i in input_mod.iter_mut().skip(pos + 1) {
                    *i += 1;
                }
                let until = pos + 1 + (rest_to_increase as usize) - input_size as usize;
                for i in input_mod.iter_mut().take(until) {
                    *i += 1;
                }
            } else {
                for i in input_mod
                    .iter_mut()
                    .skip(pos + 1)
                    .take(rest_to_increase as usize)
                {
                    *i += 1;
                }
            }
        }
        // println!("{:?}", input_mod);
    }
    return counter - previous_states.get(&input_mod).unwrap();
}

fn main() {
    let input = read_input();
    println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
