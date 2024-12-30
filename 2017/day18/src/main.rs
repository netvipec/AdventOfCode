use std::collections::VecDeque;
use std::io::{self, BufRead};

#[derive(Debug)]
enum RegisterOrNumber {
    None,
    Register(u8),
    Number(i64),
}

#[derive(Debug)]
struct Instruction {
    opcode: String,
    register: RegisterOrNumber,
    register_or_number: RegisterOrNumber,
}

type InputT = Vec<Instruction>;
type OutputT = i64;

fn read_register_or_number(data: &str) -> RegisterOrNumber {
    let first_char = data.chars().next().unwrap();
    if 'a' <= first_char && first_char <= 'z' {
        RegisterOrNumber::Register(data.as_bytes()[0])
    } else {
        RegisterOrNumber::Number(data.to_string().parse::<i64>().unwrap())
    }
}

fn get_register(rd: &RegisterOrNumber, registers: &Vec<i64>) -> i64 {
    match rd {
        RegisterOrNumber::Register(r) => *registers.get(*r as usize - 'a' as usize).unwrap(),
        RegisterOrNumber::Number(n) => *n,
        RegisterOrNumber::None => panic!("Error"),
    }
}

fn get_register_mut<'a>(rd: &RegisterOrNumber, registers: &'a mut Vec<i64>) -> &'a mut i64 {
    match rd {
        RegisterOrNumber::Register(r) => registers.get_mut(*r as usize - 'a' as usize).unwrap(),
        RegisterOrNumber::Number(_n) => panic!("Error"),
        RegisterOrNumber::None => panic!("Error"),
    }
}

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| {
            let line = x.unwrap();
            let elements = line.split(' ').collect::<Vec<&str>>();
            if elements.len() == 2 {
                return Instruction {
                    opcode: String::from(elements[0]),
                    register: read_register_or_number(elements[1]),
                    register_or_number: RegisterOrNumber::None,
                };
            } else if elements.len() == 3 {
                return Instruction {
                    opcode: String::from(elements[0]),
                    register: read_register_or_number(elements[1]),
                    register_or_number: read_register_or_number(elements[2]),
                };
            } else {
                panic!("Error")
            }
        })
        .collect();
    return input;
}

fn part1(input: &InputT) -> OutputT {
    let mut ip = 0;
    let mut last_sound_played = 0;
    let mut registers = vec![0i64; 26];

    while ip < input.len() {
        // println!("LastPlayerFrequency: {}", last_sound_played);
        // println!("Instruction: {:?}", input[ip]);
        // println!("Registers: {:?}", registers);
        match input[ip].opcode.as_str() {
            "snd" => {
                last_sound_played = get_register(&input[ip].register, &registers);
            }
            "set" => {
                *get_register_mut(&input[ip].register, &mut registers) =
                    get_register(&input[ip].register_or_number, &registers)
            }
            "add" => {
                *get_register_mut(&input[ip].register, &mut registers) +=
                    get_register(&input[ip].register_or_number, &registers)
            }
            "mul" => {
                *get_register_mut(&input[ip].register, &mut registers) *=
                    get_register(&input[ip].register_or_number, &registers)
            }
            "mod" => {
                *get_register_mut(&input[ip].register, &mut registers) %=
                    get_register(&input[ip].register_or_number, &registers)
            }
            "rcv" => {
                if get_register(&input[ip].register, &registers) != 0 {
                    return last_sound_played;
                }
            }
            "jgz" => {
                if get_register(&input[ip].register, &registers) > 0 {
                    let jump = get_register(&input[ip].register_or_number, &registers) - 1;
                    if (ip as i64) + jump < 0 {
                        panic!("Error")
                    }
                    ip = ((ip as i64) + jump) as usize;
                }
            }
            _ => panic!("Error"),
        }

        ip += 1;
    }

    return 0;
}

struct Program<'a> {
    input: &'a InputT,
    ip: usize,
    registers: Vec<i64>,
    received_queue: VecDeque<i64>,
}

fn create_program(input: &InputT, id: usize) -> Program {
    let mut p = Program {
        input: input,
        ip: 0,
        registers: vec![0; 26],
        received_queue: VecDeque::new(),
    };
    *p.registers.get_mut('p' as usize - 'a' as usize).unwrap() = id as i64;
    return p;
}

#[derive(PartialEq)]
enum State {
    WaitingForInput,
    Finished,
    Executing,
    SendData,
}

fn execute_one_instruction(program: &mut Program, other_program: &mut VecDeque<i64>) -> State {
    if program.ip >= program.input.len() {
        return State::Finished;
    }
    let mut send_data = false;
    let ip = program.ip;
    match program.input[ip].opcode.as_str() {
        "snd" => {
            let v1 = get_register(&program.input[ip].register, &program.registers);
            other_program.push_back(v1);
            send_data = true;
        }
        "set" => {
            let b = get_register(&program.input[ip].register_or_number, &program.registers);
            let a = get_register_mut(&program.input[ip].register, &mut program.registers);
            *a = b;
        }
        "add" => {
            let b = get_register(&program.input[ip].register_or_number, &program.registers);
            let a = get_register_mut(&program.input[ip].register, &mut program.registers);
            *a += b;
        }
        "mul" => {
            let b = get_register(&program.input[ip].register_or_number, &program.registers);
            let a = get_register_mut(&program.input[ip].register, &mut program.registers);
            *a *= b;
        }
        "mod" => {
            let b = get_register(&program.input[ip].register_or_number, &program.registers);
            let a = get_register_mut(&program.input[ip].register, &mut program.registers);
            *a %= b;
        }
        "rcv" => {
            if program.received_queue.len() == 0 {
                return State::WaitingForInput;
            }

            let b = program.received_queue.pop_front().unwrap();
            let a = get_register_mut(&program.input[ip].register, &mut program.registers);
            *a = b;
        }
        "jgz" => {
            if get_register(&program.input[ip].register, &program.registers) > 0 {
                let jump =
                    get_register(&program.input[ip].register_or_number, &program.registers) - 1;
                if (ip as i64) + jump < 0 {
                    panic!("Error")
                }
                program.ip = ((ip as i64) + jump) as usize;
            }
        }
        _ => panic!("Error"),
    }

    program.ip += 1;
    if send_data {
        return State::SendData;
    } else {
        return State::Executing;
    }
}

fn part2(input: &InputT) -> OutputT {
    let mut program0 = create_program(input, 0);
    let mut program1 = create_program(input, 1);
    let mut state0 = State::Executing;
    let mut state1 = State::Executing;
    let mut counter = 0;
    // let mut inst_counter = 0;

    // println!(
    //     "{} - Init -> Program 0: {}, registers: {:?}, input: {:?}",
    //     inst_counter,
    //     program0.ip + 1,
    //     program0.registers,
    //     program0.received_queue
    // );
    // println!(
    //     "{} - Init -> Program 1: {}, registers: {:?}, input: {:?}",
    //     inst_counter,
    //     program1.ip + 1,
    //     program1.registers,
    //     program1.received_queue
    // );

    loop {
        // inst_counter += 1;

        if (program1.received_queue.len() <= program0.received_queue.len()
            && state0 != State::Finished
            && state0 != State::WaitingForInput)
            || (state0 == State::WaitingForInput && program0.received_queue.len() > 0)
        {
            // println!(
            //     "{} - Pre  -> Program 0: {}, registers: {:?}, input: {:?}",
            //     inst_counter,
            //     program0.ip + 1,
            //     program0.registers,
            //     program0.received_queue
            // );
            state0 = execute_one_instruction(&mut program0, &mut program1.received_queue);
            // println!(
            //     "{} - Post -> Program 0: {}, registers: {:?}, input: {:?}",
            //     inst_counter,
            //     program0.ip + 1,
            //     program0.registers,
            //     program0.received_queue
            // );
        }
        if (program0.received_queue.len() <= program1.received_queue.len()
            && state1 != State::Finished
            && state1 != State::WaitingForInput)
            || (state1 == State::WaitingForInput && program1.received_queue.len() > 0)
        {
            // println!(
            //     "{} - Pre  -> Program 1: {}, registers: {:?}, input: {:?}",
            //     inst_counter,
            //     program1.ip + 1,
            //     program1.registers,
            //     program1.received_queue
            // );
            state1 = execute_one_instruction(&mut program1, &mut program0.received_queue);
            // println!(
            //     "{} - Post -> Program 1: {}, registers: {:?}, input: {:?}",
            //     inst_counter,
            //     program1.ip + 1,
            //     program1.registers,
            //     program1.received_queue
            // );
            if state1 == State::SendData {
                counter += 1;
            }
        }

        if (state0 == State::Finished || state0 == State::WaitingForInput)
            && (state1 == State::Finished || state1 == State::WaitingForInput)
        {
            break;
        }
    }

    return counter;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
