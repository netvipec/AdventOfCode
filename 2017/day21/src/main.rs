use std::collections::HashMap;
use std::io::{self, BufRead};

#[derive(Debug, PartialEq, Eq, Hash)]
struct Square {
    size: usize,
    number: usize,
}

type InputT = (HashMap<Square, usize>, Vec<Vec<Vec<char>>>);
type OutputT = usize;

fn get_number(square: &Vec<&str>) -> usize {
    let mut n = 0;
    square.iter().for_each(|r| {
        r.chars()
            .for_each(|c| if c == '#' { n = 2 * n + 1 } else { n = 2 * n });
    });
    return n;
}

fn read_input() -> InputT {
    let stdin = io::stdin();
    let mut input: InputT = (HashMap::new(), Vec::new());
    stdin.lock().lines().for_each(|x| {
        let line = x.unwrap();
        let parts: Vec<&str> = line.split(" => ").collect();
        assert_eq!(parts.iter().count(), 2);

        let square1: Vec<&str> = parts[0].split('/').collect();
        let square2: Vec<Vec<char>> = parts[1].split('/').map(|a| a.chars().collect()).collect();

        let s = square1.len();
        let n = get_number(&square1);

        input.0.insert(Square { size: s, number: n }, input.1.len());
        input.1.push(square2);
    });
    return input;
}

fn get_value(grid: &Vec<Vec<char>>, x: usize, y: usize, size: usize, t: usize) -> usize {
    let mut n = 0;
    match t {
        0 => {
            for i in 0..size {
                for j in 0..size {
                    if grid[x + i][y + j] == '#' {
                        n = 2 * n + 1
                    } else {
                        n = 2 * n
                    }
                }
            }
        }
        1 => {
            for i in (0..size).rev() {
                for j in (0..size).rev() {
                    if grid[x + i][y + j] == '#' {
                        n = 2 * n + 1
                    } else {
                        n = 2 * n
                    }
                }
            }
        }
        2 => {
            for i in 0..size {
                for j in (0..size).rev() {
                    if grid[x + i][y + j] == '#' {
                        n = 2 * n + 1
                    } else {
                        n = 2 * n
                    }
                }
            }
        }
        3 => {
            for i in (0..size).rev() {
                for j in 0..size {
                    if grid[x + i][y + j] == '#' {
                        n = 2 * n + 1
                    } else {
                        n = 2 * n
                    }
                }
            }
        }
        4 => {
            for i in 0..size {
                for j in 0..size {
                    if grid[x + j][y + i] == '#' {
                        n = 2 * n + 1
                    } else {
                        n = 2 * n
                    }
                }
            }
        }
        5 => {
            for i in (0..size).rev() {
                for j in (0..size).rev() {
                    if grid[x + j][y + i] == '#' {
                        n = 2 * n + 1
                    } else {
                        n = 2 * n
                    }
                }
            }
        }
        6 => {
            for i in 0..size {
                for j in (0..size).rev() {
                    if grid[x + j][y + i] == '#' {
                        n = 2 * n + 1
                    } else {
                        n = 2 * n
                    }
                }
            }
        }
        7 => {
            for i in (0..size).rev() {
                for j in 0..size {
                    if grid[x + j][y + i] == '#' {
                        n = 2 * n + 1
                    } else {
                        n = 2 * n
                    }
                }
            }
        }
        _ => panic!(),
    }
    return n;
}

fn set_value(grid: &mut Vec<Vec<char>>, x: usize, y: usize, new_value: &Vec<Vec<char>>) {
    for xi in 0..new_value.len() {
        for yi in 0..new_value.len() {
            grid[x + xi][y + yi] = new_value[xi][yi];
        }
    }
}

fn evolve_grid(grid: &Vec<Vec<char>>, input: &InputT) -> Vec<Vec<char>> {
    let increment = if grid.len() % 2 == 0 { 2 } else { 3 };
    let new_size = grid.len() / increment * (increment + 1);
    let mut new_grid: Vec<Vec<char>> = vec![vec!['X'; new_size]; new_size];

    for x in (0..grid.len()).step_by(increment) {
        for y in (0..grid[x].len()).step_by(increment) {
            let mut index = input.1.len() + 1;
            for t in 0..8 {
                let value = get_value(&grid, x, y, increment, t);
                index = match input.0.get_key_value(&Square {
                    size: increment,
                    number: value,
                }) {
                    Some(x) => *x.1,
                    None => continue,
                };
                break;
            }

            let new_x = (x / increment) * (increment + 1);
            let new_y = (y / increment) * (increment + 1);
            set_value(&mut new_grid, new_x, new_y, &input.1[index]);
        }
    }

    return new_grid;
}

fn part1(input: &InputT) -> OutputT {
    let init_pattern = vec![
        vec!['.', '#', '.'],
        vec!['.', '.', '#'],
        vec!['#', '#', '#'],
    ];

    let mut grid = init_pattern.clone();

    for _iterations in 0..5 {
        grid = evolve_grid(&grid, input);
        // grid.iter().for_each(|r| {
        //     r.iter().for_each(|c| print!("{}", c));
        //     println!();
        // });
        // println!();
    }

    let mut pixel_on = 0;
    grid.iter()
        .for_each(|r| pixel_on += r.iter().filter(|c| **c == '#').count());
    return pixel_on;
}

fn part2(input: &InputT) -> OutputT {
    let init_pattern = vec![
        vec!['.', '#', '.'],
        vec!['.', '.', '#'],
        vec!['#', '#', '#'],
    ];

    let mut grid = init_pattern.clone();

    for _iterations in 0..18 {
        grid = evolve_grid(&grid, input);
        // grid.iter().for_each(|r| {
        //     r.iter().for_each(|c| print!("{}", c));
        //     println!();
        // });
        // println!();
    }

    let mut pixel_on = 0;
    grid.iter()
        .for_each(|r| pixel_on += r.iter().filter(|c| **c == '#').count());
    return pixel_on;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
