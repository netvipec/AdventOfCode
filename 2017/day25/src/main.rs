use std::char;
use std::collections::VecDeque;
use std::io::{self, BufRead};

#[derive(Debug)]
enum MoveDirection {
    Left,
    Right,
}

#[derive(Debug)]
struct State {
    write: bool,
    move_direction: MoveDirection,
    transition_state: char,
}

#[derive(Debug)]
struct FullState {
    states: [State; 2],
}

type InputT = ((char, usize), Vec<FullState>);
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let mut iterator = stdin.lock().lines();
    let line_a = iterator.next().unwrap().unwrap();
    let start_state = line_a.chars().skip(15).take(1).next().unwrap();
    // println!("{}", start_state);
    let line_b = iterator.next().unwrap().unwrap();
    let mut steps_it = line_b[36..].split(' ');
    let steps = steps_it.next().unwrap().parse::<usize>().unwrap();
    // println!("{}", steps);
    iterator.next();
    let mut input: InputT = ((start_state, steps), Vec::new());

    loop {
        let line1 = match iterator.next() {
            Some(x) => x.unwrap(),
            None => break,
        };
        let _state_actual = &line1[9..];
        // println!("{}", state_actual);

        input.1.push(FullState {
            states: [
                State {
                    write: false,
                    move_direction: MoveDirection::Left,
                    transition_state: ' ',
                },
                State {
                    write: false,
                    move_direction: MoveDirection::Left,
                    transition_state: ' ',
                },
            ],
        });

        for i in 0..2 {
            let _line2 = iterator.next().unwrap().unwrap();
            let line3 = iterator.next().unwrap().unwrap();
            let state_actual = line3.chars().skip(22).take(1).next().unwrap();
            let line4 = iterator.next().unwrap().unwrap();
            let dir_actual = &line4[27..];
            let line5 = iterator.next().unwrap().unwrap();
            let transition_actual = line5.chars().skip(26).take(1).next().unwrap();

            // println!("{} {:?} {}", state_actual, dir_actual, transition_actual);

            let mut last = input.1.last_mut().unwrap();
            last.states[i].write = if state_actual == '1' { true } else { false };
            last.states[i].move_direction = if dir_actual.starts_with("left") {
                MoveDirection::Left
            } else {
                MoveDirection::Right
            };
            last.states[i].transition_state = transition_actual;
        }
        iterator.next();
    }
    return input;
}

fn part1(input: &InputT) -> OutputT {
    let mut cursor = 0;
    let mut state = (input.0).0 as usize - 'A' as usize;
    let mut tape: VecDeque<bool> = VecDeque::new();
    tape.push_back(false);

    for _steps in 0..(input.0).1 {
        // println!(
        //     "{:?} cursor: {} state: {}",
        //     tape,
        //     cursor,
        //     char::from_u32(state as u32 + 'A' as u32).unwrap()
        // );
        let tape_value = tape.get_mut(cursor).unwrap();
        let current_tape_value = if *tape_value { 1 } else { 0 };
        let current_state = input.1.get(state).unwrap();
        let state_desc = &current_state.states[current_tape_value];
        *tape_value = state_desc.write;
        state = state_desc.transition_state as usize - 'A' as usize;
        match state_desc.move_direction {
            MoveDirection::Left => {
                if cursor > 0 {
                    cursor -= 1;
                } else {
                    tape.push_front(false);
                }
            }
            MoveDirection::Right => {
                cursor += 1;
                if cursor == tape.len() {
                    tape.push_back(false);
                }
            }
        }
    }
    return tape.iter().filter(|e| **e == true).count();
}

fn part2(_input: &InputT) -> OutputT {
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
