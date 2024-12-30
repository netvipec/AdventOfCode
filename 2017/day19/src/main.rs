use std::io::{self, BufRead};

type InputT = Vec<Vec<char>>;
type Output1T = String;
type Output2T = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| {
            let line = x.unwrap();
            line.chars().collect()
        })
        .collect();
    return input;
}

fn get_new_pos(input: &InputT, row: i64, col: i64, index: usize) -> Option<(i64, i64, char)> {
    let dx = vec![0, 0, 1, -1];
    let dy = vec![1, -1i64, 0, 0];
    let width = input[0].len() as i64;
    let height = input.len() as i64;

    let nrow = row + dx[index];
    let ncol = col + dy[index];
    if 0 <= nrow && nrow <= height && 0 <= ncol && ncol <= width {
        let r = nrow as usize;
        let c = ncol as usize;
        if input[r][c] == '|'
            || input[r][c] == '-'
            || input[r][c] == '+'
            || ('A' as u8 <= input[r][c] as u8 && input[r][c] as u8 <= 'Z' as u8)
        {
            if input[r][c] == '+' {
                return Some((nrow, ncol, '\0'));
            } else if 'A' as u8 <= input[r][c] as u8 && input[r][c] as u8 <= 'Z' as u8 {
                return Some((nrow, ncol, input[r][c]));
            } else {
                return Some((nrow, ncol, '\0'));
            }
        }
    }
    return None;
}

fn move_path(input: &InputT) -> (Output1T, Output2T) {
    let mut row = 0i64;
    let mut col = input[0].iter().position(|x| *x == '|').unwrap() as i64;
    let mut solution = String::from("");
    let mut dir = 3;
    let mut turn = false;
    let mut counter = 0;
    loop {
        counter += 1;
        let mut moved = false;
        let begin = dir / 2 * 2;
        let end = begin + 2;
        for i in begin..end {
            if i == dir && !turn {
                continue;
            }
            match get_new_pos(input, row, col, i) {
                Some(x) => {
                    moved = true;
                    row = x.0;
                    col = x.1;
                    solution.push(x.2);
                    // println!(
                    //     "({},{}) -> {} = {}",
                    //     row, col, solution, input[row as usize][col as usize]
                    // );
                    if input[row as usize][col as usize] == '+' {
                        turn = true;
                        if i < 2 {
                            dir = (i + 1) % 2 + 2;
                        } else {
                            dir = (i + 1) % 2;
                        }
                    } else {
                        turn = false;
                        if i < 2 {
                            dir = (i + 1) % 2;
                        } else {
                            dir = (i + 1) % 2 + 2;
                        }
                    }
                    break;
                }
                None => {}
            };
        }

        if !moved {
            break;
        }
    }

    // println!("({},{})", row, col);

    return (solution, counter);
}

fn part1(input: &InputT) -> Output1T {
    return move_path(input).0;
}

fn part2(input: &InputT) -> Output2T {
    return move_path(input).1;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
