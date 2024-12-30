use std::collections::HashSet;
use std::io::{self, BufRead};

type InputT = Vec<String>;
type OutputT = i64;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin.lock().lines().map(|x| x.unwrap()).collect();
    return input;
}

fn part1(input: &InputT) -> OutputT {
    let mut res = 0;
    let mut used_words = HashSet::new();
    for line in input {
        used_words.clear();
        let words = line.split(' ');
        let mut bad = false;
        for word in words {
            if !used_words.insert(word) {
                bad = true;
                break;
            }
        }

        if !bad {
            res += 1;
        }
    }
    return res;
}

fn part2(input: &InputT) -> OutputT {
    let mut res = 0;
    let mut used_words = HashSet::new();
    for line in input {
        used_words.clear();
        let words = line.split(' ');
        let mut bad = false;
        for word in words {
            let mut letters = vec![0; 26];
            for l in word.bytes() {
                letters[(l - 'a' as u8) as usize] += 1;
            }
            if !used_words.insert(letters) {
                bad = true;
                break;
            }
        }

        if !bad {
            res += 1;
        }
    }
    return res;
}

fn main() {
    let input = read_input();

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
