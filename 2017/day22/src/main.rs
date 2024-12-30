use std::collections::{HashMap, HashSet};
use std::io::{self, BufRead};

type InputT = Vec<Vec<char>>;
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| x.unwrap().chars().collect())
        .collect();

    return input;
}

fn get_grid(input: &InputT) -> HashSet<(i64, i64)> {
    let mut grid = HashSet::new();

    let mut r = -(input.len() as i64 / 2);
    input.iter().for_each(|rr| {
        let mut c = -(input.len() as i64 / 2);
        rr.iter().for_each(|cc| {
            if *cc == '#' {
                grid.insert((r, c));
            }
            c += 1;
        });
        r += 1;
    });
    return grid;
}

// 0: up, 1: right, 2: down, 3: left

fn turn_left(direction: usize) -> usize {
    return if direction == 0 { 3 } else { direction - 1 };
}
fn turn_right(direction: usize) -> usize {
    return if direction == 3 { 0 } else { direction + 1 };
}

fn move_virus(x: &mut i64, y: &mut i64, direction: usize) {
    match direction {
        0 => {
            *x -= 1;
        }
        1 => {
            *y += 1;
        }
        2 => {
            *x += 1;
        }
        3 => {
            *y -= 1;
        }
        _ => panic!(),
    }
}

fn part1(input: &InputT) -> OutputT {
    let mut grid = get_grid(input);

    let mut x = 0i64;
    let mut y = 0i64;
    let mut direction = 0;

    let mut infested = 0;
    let mut _cleaned = 0;
    for _bursts in 0..10000 {
        if grid.contains(&(x, y)) {
            direction = turn_right(direction);
            grid.remove(&(x, y));
            _cleaned += 1;
        } else {
            direction = turn_left(direction);
            grid.insert((x, y));
            infested += 1;
        }
        move_virus(&mut x, &mut y, direction);
    }

    return infested;
}

// 0- Clean, 1- Weakened, 2- Infected, 3- Flagged

fn part2(input: &InputT) -> OutputT {
    let grid_base = get_grid(input);
    let mut grid: HashMap<(i64, i64), usize> = grid_base.iter().map(|e| (*e, 2)).collect();

    let mut x = 0i64;
    let mut y = 0i64;
    let mut direction = 0;

    let mut infested = 0;
    let mut _cleaned = 0;
    for _bursts in 0..10000000 {
        match grid.get_mut(&(x, y)) {
            Some(v) => {
                let new_v = match v {
                    0 => {
                        direction = turn_left(direction);
                        1
                    }
                    1 => {
                        infested += 1;
                        2
                    }
                    2 => {
                        direction = turn_right(direction);
                        3
                    }
                    3 => {
                        direction = turn_right(turn_right(direction));
                        _cleaned += 1;
                        0
                    }
                    _ => panic!(),
                };
                *v = new_v;
            }
            None => {
                direction = turn_left(direction);
                grid.insert((x, y), 1);
            }
        }
        move_virus(&mut x, &mut y, direction);
    }

    return infested;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
