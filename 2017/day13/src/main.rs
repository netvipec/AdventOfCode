use std::io::{self, BufRead};

type InputT = Vec<(usize, usize)>;
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| {
            let line = x.unwrap();
            let mut p = line.split(": ");
            let f = p.next().unwrap().parse::<usize>().unwrap();
            let s = p.next().unwrap().parse::<usize>().unwrap();
            (f, s)
        })
        .collect();
    return input;
}

fn severity(firewall: &(usize, usize)) -> usize {
    return firewall.0 * firewall.1;
}

fn part1(input: &InputT) -> OutputT {
    let mut severity_trip = 0;
    for f in input {
        if f.1 == 1 {
            severity_trip += severity(f);
        } else {
            if f.0 % (2 * f.1 - 2) == 0 {
                severity_trip += severity(f);
            }
        }
    }

    return severity_trip;
}

fn detected(firewalls: &InputT, delay: usize) -> bool {
    for f in firewalls {
        if f.0 == 0 {
            if delay % f.1 == 0 {
                return true;
            }
        }
        match f.1 {
            1 => return true,
            _ => {
                if (f.0 + delay) % (2 * f.1 - 2) == 0 {
                    return true;
                }
            }
        }
    }
    return false;
}

fn part2(input: &InputT) -> OutputT {
    // can be use Chinese remainder theorem.
    for i in 1..2000000 {
        if !detected(input, 2 * i) {
            return 2 * i;
        }
    }
    return 0;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
