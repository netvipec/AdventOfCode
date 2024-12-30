use std::collections::HashMap;
use std::io::{self, BufRead};

type InputT = Vec<String>;
type Output1T = String;
type Output2T = i64;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin.lock().lines().map(|x| x.unwrap()).collect();
    return input;
}

fn part1(input: &InputT) -> Output1T {
    let mut programs = HashMap::new();
    for line in input {
        if line.contains("->") {
            let parts: Vec<&str> = line.split(" -> ").collect();
            if parts.len() > 1 {
                let dependent: Vec<&str> = parts[1].split(", ").collect();

                for d in dependent {
                    *programs.entry(d).or_insert(0) += 1;
                }
            }
        }
        let d: Vec<&str> = line.split(" ").collect();
        *programs.entry(d[0]).or_insert(0) += 1;
    }

    let root: Vec<(&str, i64)> = programs
        .iter()
        .filter(|p| *p.1 == 1)
        .map(|p| (*p.0, *p.1))
        .collect();
    // println!("{:?}", programs);
    return root[0].0.to_string();
}

fn find_unbalance(
    programs: &Vec<(&str, i64)>,
    programs_tree: &Vec<Vec<usize>>,
    root_idx: usize,
) -> (i64, i64) {
    if programs_tree[root_idx].len() == 0 {
        return (programs[root_idx].1, 0);
    }

    let mut d_balance = Vec::new();
    for d in &programs_tree[root_idx] {
        let balance = find_unbalance(programs, programs_tree, *d);
        if balance.1 != 0 {
            return (0, balance.1);
        }
        d_balance.push(balance.0);
    }
    let first_count = d_balance.iter().filter(|&d| *d == d_balance[0]).count();
    if first_count != d_balance.len() {
        if first_count == 1 {
            let fix = d_balance[1] - d_balance[0];
            let node_weigth = programs[programs_tree[root_idx][0]].1;
            return (
                d_balance.len() as i64 * (node_weigth + fix),
                node_weigth + fix,
            );
        } else {
        }
    }

    return (d_balance.iter().sum::<i64>() + programs[root_idx].1, 0);
}

fn part2(input: &InputT) -> Output2T {
    let mut programs = Vec::new();
    for line in input {
        let d: Vec<&str> = line.split(" ").collect();
        let weight_start = line.find('(').unwrap();
        let weight_end = line.find(')').unwrap();
        let weight: i64 = line[(weight_start + 1)..(weight_end)].parse().unwrap();
        programs.push((d[0], weight));
    }

    let mut programs_tree = vec![Vec::new(); programs.len()];
    for line in input {
        if line.contains("->") {
            let r: Vec<&str> = line.split(" ").collect();
            let r_idx = programs.iter().position(|p| *p.0 == *r[0]).unwrap();

            let parts: Vec<&str> = line.split(" -> ").collect();
            if parts.len() > 1 {
                let dependent: Vec<&str> = parts[1].split(", ").collect();

                for d in dependent {
                    let d_idx = programs.iter().position(|p| *p.0 == *d).unwrap();
                    programs_tree[r_idx].push(d_idx);
                }
            }
        }
    }

    let root = part1(input);
    let root_idx = programs.iter().position(|p| *p.0 == *root).unwrap();

    let sol = find_unbalance(&programs, &programs_tree, root_idx);

    // println!("{:?}", programs);
    // println!("{:?}", programs_tree);
    // println!(
    //     "{:?}: {:?} -> {:?}",
    //     root, root_idx, programs_tree[root_idx]
    // );

    return sol.1;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
