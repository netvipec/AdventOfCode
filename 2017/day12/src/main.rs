use itertools::Itertools;
use std::cmp;
use std::collections::HashSet;
use std::io::{self, BufRead};
use std::mem;

type InputT = Vec<(usize, Vec<usize>)>;
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| {
            let line = x.unwrap();
            let mut p = line.split("<->");
            let f = p.next().unwrap().trim().parse::<usize>().unwrap();
            let s: Vec<usize> = p
                .next()
                .unwrap()
                .split(',')
                .map(|y| y.trim().parse::<usize>().unwrap())
                .collect();
            (f, s)
        })
        .collect();
    return input;
}

fn part1(input: &InputT) -> OutputT {
    let mut programs = HashSet::new();
    let mut explore = HashSet::new();
    let mut new_explore = HashSet::new();
    explore.insert(0);
    while explore.len() > 0 {
        for ep in &explore {
            for aep in &input[*ep].1 {
                if programs.insert(aep) {
                    new_explore.insert(*aep);
                }
            }
        }

        mem::swap(&mut explore, &mut new_explore);
        new_explore.clear();
    }
    // println!("{:?}", programs);
    return programs.len();
}

fn find(mut programs: &mut Vec<usize>, index: usize, possible_index: usize) -> usize {
    if programs[index] == index {
        return index;
    }

    let parent = programs[index];
    let new_index = find(&mut programs, parent, possible_index);
    programs[index] = cmp::min(new_index, possible_index);
    return programs[index];
}

fn part2(input: &InputT) -> OutputT {
    let mut programs = vec![0; (input.last().unwrap().0 + 1) as usize];
    for i in 0..programs.len() {
        programs[i] = i;
    }
    let mut changed = true;
    while changed {
        changed = false;
        for p in input {
            let value = cmp::min(
                p.1.iter()
                    .map(|x| find(&mut programs, *x, p.0))
                    .min()
                    .unwrap(),
                find(&mut programs, p.0, p.0),
            );

            if value != programs[p.0] {
                programs[p.0] = value;
                changed = true;
            }
            for pp in &p.1 {
                if value != programs[*pp] {
                    programs[*pp] = value;
                    changed = true;
                }
            }
        }
    }
    return programs.into_iter().unique().count();
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
