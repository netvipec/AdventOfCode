use std::collections::HashMap;
use std::io::{self, BufRead};

#[derive(Debug)]
struct Expr {
    modify_variable: String,
    increment: bool,
    modify_value: i64,
    condition_variable: String,
    operator: String,
    condition_value: i64,
}

type InputT = Vec<Expr>;
type OutputT = i64;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| {
            let line = x.unwrap();
            let parts: Vec<&str> = line.split(" ").collect();
            return Expr {
                modify_variable: parts[0].to_string(),
                increment: parts[1] == "inc",
                modify_value: parts[2].parse::<i64>().unwrap(),
                condition_variable: parts[4].to_string(),
                operator: parts[5].to_string(),
                condition_value: parts[6].parse::<i64>().unwrap(),
            };
        })
        .collect();
    return input;
}

fn part1(input: &InputT) -> OutputT {
    let mut registry = HashMap::new();
    for inst in input {
        registry.entry(&inst.modify_variable).or_insert(0i64);
        registry.entry(&inst.condition_variable).or_insert(0i64);
        let condition_true = match &inst.operator[..] {
            "<" => *registry.get(&inst.condition_variable).unwrap() < inst.condition_value,
            ">" => *registry.get(&inst.condition_variable).unwrap() > inst.condition_value,
            "<=" => *registry.get(&inst.condition_variable).unwrap() <= inst.condition_value,
            ">=" => *registry.get(&inst.condition_variable).unwrap() >= inst.condition_value,
            "==" => *registry.get(&inst.condition_variable).unwrap() == inst.condition_value,
            "!=" => *registry.get(&inst.condition_variable).unwrap() != inst.condition_value,
            _ => panic!(),
        };
        if condition_true {
            if inst.increment {
                *registry.get_mut(&inst.modify_variable).unwrap() += inst.modify_value;
            } else {
                *registry.get_mut(&inst.modify_variable).unwrap() -= inst.modify_value;
            }
        }
    }
    let max_registry = registry.into_iter().max_by_key(|r| r.1).unwrap();
    return max_registry.1;
}

fn part2(input: &InputT) -> OutputT {
    let mut registry = HashMap::new();
    let mut max_registry = i64::MIN;
    for inst in input {
        registry.entry(&inst.modify_variable).or_insert(0i64);
        registry.entry(&inst.condition_variable).or_insert(0i64);
        let condition_true = match &inst.operator[..] {
            "<" => *registry.get(&inst.condition_variable).unwrap() < inst.condition_value,
            ">" => *registry.get(&inst.condition_variable).unwrap() > inst.condition_value,
            "<=" => *registry.get(&inst.condition_variable).unwrap() <= inst.condition_value,
            ">=" => *registry.get(&inst.condition_variable).unwrap() >= inst.condition_value,
            "==" => *registry.get(&inst.condition_variable).unwrap() == inst.condition_value,
            "!=" => *registry.get(&inst.condition_variable).unwrap() != inst.condition_value,
            _ => panic!(),
        };
        if condition_true {
            if inst.increment {
                *registry.get_mut(&inst.modify_variable).unwrap() += inst.modify_value;
            } else {
                *registry.get_mut(&inst.modify_variable).unwrap() -= inst.modify_value;
            }
        }
        let max_actual_registry = registry.iter().max_by_key(|r| r.1).unwrap();
        if *max_actual_registry.1 > max_registry {
            max_registry = *max_actual_registry.1;
        }
    }
    return max_registry;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
