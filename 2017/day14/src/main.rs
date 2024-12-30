use std::collections::HashSet;
use std::io::{self, BufRead};

type InputT = Vec<u8>;
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .next()
        .unwrap()
        .unwrap()
        .bytes()
        .collect();
    return input;
}

fn knot_hash(input: &Vec<u8>) -> String {
    let add_input = vec![17, 31, 73, 47, 23];
    let mut new_input = vec![0; input.len() + add_input.len()];
    for i in 0..input.len() {
        new_input[i] = input[i] as usize;
    }
    for i in 0..add_input.len() {
        new_input[input.len() + i] = add_input[i];
    }

    let mut circular = vec![0; 256];
    for i in 0..circular.len() {
        circular[i] = i;
    }
    let circular_size = circular.len();

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

fn count_bits(input: &str) -> usize {
    let table = vec![0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4];
    let mut r = 0;
    for b in input.as_bytes() {
        let i = (|| {
            if '0' as u8 <= *b && *b <= '9' as u8 {
                *b - '0' as u8
            } else {
                10 + *b - 'a' as u8
            }
        })();

        r += table[i as usize];
    }
    return r;
}

fn bits(input: &str) -> Vec<usize> {
    let table = vec![
        [0, 0, 0, 0],
        [0, 0, 0, 1],
        [0, 0, 1, 0],
        [0, 0, 1, 1],
        [0, 1, 0, 0],
        [0, 1, 0, 1],
        [0, 1, 1, 0],
        [0, 1, 1, 1],
        [1, 0, 0, 0],
        [1, 0, 0, 1],
        [1, 0, 1, 0],
        [1, 0, 1, 1],
        [1, 1, 0, 0],
        [1, 1, 0, 1],
        [1, 1, 1, 0],
        [1, 1, 1, 1],
    ];
    let mut r = Vec::new();
    for b in input.as_bytes() {
        let i = (|| {
            if '0' as u8 <= *b && *b <= '9' as u8 {
                *b - '0' as u8
            } else {
                10 + *b - 'a' as u8
            }
        })();

        table[i as usize].iter().for_each(|x| r.push(*x));
    }
    return r;
}

fn part1(input: &InputT) -> OutputT {
    let mut active = 0;
    for i in 0..128 {
        let mut row_input = input.clone();
        "-".as_bytes().iter().for_each(|x| row_input.push(*x));
        i.to_string()
            .as_bytes()
            .iter()
            .for_each(|x| row_input.push(*x));
        let h = knot_hash(&row_input);

        active += count_bits(&h);
        // println!(
        //     "{:?} -> {} -> {:?}",
        //     String::from_utf8(row_input).unwrap(),
        //     active,
        //     h
        // );
    }

    return active;
}

fn part2(input: &InputT) -> OutputT {
    let mut active = Vec::new();
    for i in 0..128 {
        let mut row_input = input.clone();
        "-".as_bytes().iter().for_each(|x| row_input.push(*x));
        i.to_string()
            .as_bytes()
            .iter()
            .for_each(|x| row_input.push(*x));
        let h = knot_hash(&row_input);

        active.push(bits(&h));
    }
    // active.iter().for_each(|x| println!("{:?}", x));

    let mut counter = 1usize;
    for r in active.iter_mut() {
        for c in r {
            if *c > 0 {
                *c = counter;
                counter += 1;
            }
        }
    }

    let mut changed = true;
    while changed {
        changed = false;

        for ri in 0..active.len() {
            for ci in 0..active[ri].len() {
                if active[ri][ci] == 0 {
                    continue;
                }

                if ci > 0 {
                    let le = active[ri][ci - 1];
                    let th = &mut active[ri][ci];
                    if le > 0 && le < *th {
                        *th = le;
                        changed = true;
                    }
                }
                if ri > 0 {
                    let up = active[ri - 1][ci];
                    let th = &mut active[ri][ci];
                    if up > 0 && up < *th {
                        *th = up;
                        changed = true;
                    }
                }
                if ci < active[ri].len() - 1 {
                    let rr = active[ri][ci + 1];
                    let th = &mut active[ri][ci];
                    if rr > 0 && rr < *th {
                        *th = rr;
                        changed = true;
                    }
                }
                if ri < active.len() - 1 {
                    let dd = active[ri + 1][ci];
                    let th = &mut active[ri][ci];
                    if dd > 0 && dd < *th {
                        *th = dd;
                        changed = true;
                    }
                }
            }
        }
    }

    // println!();
    // active.iter().for_each(|x| println!("{:?}", x));

    let mut groups = HashSet::new();
    active.iter().for_each(|x| {
        (*x).iter().for_each(|y| {
            let _ = groups.insert(*y);
        })
    });

    return groups.len() - 1;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
