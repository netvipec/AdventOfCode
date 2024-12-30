use std::collections::HashMap;
use std::io::{self, BufRead};

#[derive(Debug)]
enum ProgramModifications {
    Skip(usize),
    Exchange(usize, usize),
    Partner(char, char),
}

type InputT = Vec<ProgramModifications>;
type OutputT = String;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .next()
        .unwrap()
        .unwrap()
        .split(",")
        .map(|x| {
            let pm = x.to_string();
            let s = pm.find('s');
            let x = pm.find('x');
            let p = pm.find('p');

            if s.is_some() {
                let n = pm[1..].parse::<usize>().unwrap();
                return ProgramModifications::Skip(n);
            } else if x.is_some() {
                let m = pm.find('/').unwrap();
                let f = pm[1..m].parse::<usize>().unwrap();
                let s = pm[(m + 1)..].parse::<usize>().unwrap();
                return ProgramModifications::Exchange(f, s);
            } else if p.is_some() {
                let m = pm.find('/').unwrap();
                let f = pm.chars().skip(1).next().unwrap();
                let s = pm.chars().skip(m + 1).next().unwrap();
                return ProgramModifications::Partner(f, s);
            } else {
                panic!("Parsing error!!!");
            }
        })
        .collect();
    return input;
}

fn one_dance(input: &InputT, programs: &mut Vec<char>) {
    for pm in input {
        match pm {
            ProgramModifications::Skip(n) => programs.rotate_right(*n),
            ProgramModifications::Exchange(p1, p2) => programs.swap(*p1, *p2),
            ProgramModifications::Partner(e1, e2) => {
                let p1 = programs.iter().position(|x| *x == *e1).unwrap();
                let p2 = programs.iter().position(|x| *x == *e2).unwrap();

                programs.swap(p1, p2);
            }
        }
    }
}

fn part1(input: &InputT) -> OutputT {
    // let mut programs = vec!['a', 'b', 'c', 'd', 'e'];
    let mut programs = vec![
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    ];

    one_dance(input, &mut programs);

    return programs.into_iter().collect();
}

fn part2(input: &InputT) -> OutputT {
    // let mut programs = vec!['a', 'b', 'c', 'd', 'e'];
    let mut programs = vec![
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    ];

    let mut dance_state = HashMap::new();
    while !dance_state.contains_key(&programs) {
        dance_state.insert(programs.clone(), dance_state.len());
        one_dance(input, &mut programs);
    }

    // println!("{:?}", &programs);

    let close_cycle_dance_number = dance_state.get(&programs).unwrap();
    // println!("{}", close_cycle_dance_number);

    let dances = 1000000000 - close_cycle_dance_number;
    let dances_cycle_rest = dances % (dance_state.len() - close_cycle_dance_number);
    // println!("{}",into_iter().collect() dances_cycle_rest);

    let result = dance_state
        .iter()
        .find(|x| *x.1 == dances_cycle_rest + close_cycle_dance_number)
        .unwrap();

    return result.0.into_iter().collect();
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
