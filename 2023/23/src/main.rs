use std::cmp;
use std::collections::HashSet;
use std::io::{self, BufRead};

#[derive(Debug, Clone, Eq, PartialEq, Hash, Copy)]
struct Pos {
    x: i32,
    y: i32,
}

// #[derive(Debug, Clone)]
// enum Direction {
//     Up,
//     Rigth,
//     Down,
//     Left,
// }

type InputT = Vec<Vec<char>>;
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let inputs: InputT = stdin
        .lock()
        .lines()
        .map(|line| line.unwrap().chars().collect())
        .collect();
    inputs
}

fn is_valid_cell(pos: Pos, grid: &InputT) -> bool {
    return 0 <= pos.x
        && pos.x < grid.len() as i32
        && 0 <= pos.y
        && pos.y < grid.first().unwrap().len() as i32;
}

fn dfs(pos: Pos, seen: &mut HashSet<Pos>, grid: &InputT, end: Pos, length: usize) -> OutputT {
    if pos == end {
        return length;
    }

    seen.insert(pos);

    let mut best = 0;
    if grid[pos.x as usize][pos.y as usize] == '.' {
        {
            let np_u = Pos {
                x: pos.x - 1,
                y: pos.y,
            };
            if is_valid_cell(np_u, grid)
                && grid[np_u.x as usize][np_u.y as usize] != '#'
                && !seen.contains(&np_u)
            {
                best = cmp::max(best, dfs(np_u, seen, grid, end, length + 1))
            }
        }
        {
            let np_r = Pos {
                x: pos.x,
                y: pos.y + 1,
            };
            if is_valid_cell(np_r, grid)
                && grid[np_r.x as usize][np_r.y as usize] != '#'
                && !seen.contains(&np_r)
            {
                best = cmp::max(best, dfs(np_r, seen, grid, end, length + 1))
            }
        }
        {
            let np_d = Pos {
                x: pos.x + 1,
                y: pos.y,
            };
            if is_valid_cell(np_d, grid)
                && grid[np_d.x as usize][np_d.y as usize] != '#'
                && !seen.contains(&np_d)
            {
                best = cmp::max(best, dfs(np_d, seen, grid, end, length + 1))
            }
        }
        {
            let np_l = Pos {
                x: pos.x,
                y: pos.y - 1,
            };
            if is_valid_cell(np_l, grid)
                && grid[np_l.x as usize][np_l.y as usize] != '#'
                && !seen.contains(&np_l)
            {
                best = cmp::max(best, dfs(np_l, seen, grid, end, length + 1))
            }
        }
    } else {
        let new_best = match grid[pos.x as usize][pos.y as usize] {
            '^' => {
                let np_u = Pos {
                    x: pos.x - 1,
                    y: pos.y,
                };
                if seen.contains(&np_u) {
                    best
                } else {
                    dfs(np_u, seen, grid, end, length + 1)
                }
            }
            '>' => {
                let np_r = Pos {
                    x: pos.x,
                    y: pos.y + 1,
                };
                if seen.contains(&np_r) {
                    best
                } else {
                    dfs(np_r, seen, grid, end, length + 1)
                }
            }
            'v' => {
                let np_d = Pos {
                    x: pos.x + 1,
                    y: pos.y,
                };
                if seen.contains(&np_d) {
                    best
                } else {
                    dfs(np_d, seen, grid, end, length + 1)
                }
            }
            '<' => {
                let np_l = Pos {
                    x: pos.x,
                    y: pos.y - 1,
                };
                if seen.contains(&np_l) {
                    best
                } else {
                    dfs(np_l, seen, grid, end, length + 1)
                }
            }
            _ => panic!(),
        };
        best = cmp::max(best, new_best);
    }

    seen.remove(&pos);

    best
}

fn part1(input: &InputT) -> OutputT {
    let start = Pos {
        x: 0,
        y: input
            .first()
            .unwrap()
            .iter()
            .position(|&c| c == '.')
            .unwrap() as i32,
    };
    let end = Pos {
        x: input.len() as i32 - 1,
        y: input
            .last()
            .unwrap()
            .iter()
            .position(|&c| c == '.')
            .unwrap() as i32,
    };

    let mut seen = HashSet::new();
    dfs(start, &mut seen, input, end, 0)
}

fn part2(input: &InputT) -> OutputT {
    let start = Pos {
        x: 0,
        y: input
            .first()
            .unwrap()
            .iter()
            .position(|&c| c == '.')
            .unwrap() as i32,
    };
    let end = Pos {
        x: input.len() as i32 - 1,
        y: input
            .last()
            .unwrap()
            .iter()
            .position(|&c| c == '.')
            .unwrap() as i32,
    };

    let mut grid = input.clone();
    for row in grid.iter_mut() {
        for cel in row.iter_mut() {
            if *cel != '#' {
                *cel = '.';
            }
        }
    }

    let mut seen = HashSet::new();
    dfs(start, &mut seen, &grid, end, 0)
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
