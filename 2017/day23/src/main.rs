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
    let mut registers = vec![0i64; 8];
    let mut mul_count = 0;

    while ip < input.len() {
        // println!("LastPlayerFrequency: {}", last_sound_played);
        // println!("Instruction: {:?}", input[ip]);
        // println!("Registers: {:?}", registers);
        match input[ip].opcode.as_str() {
            "set" => {
                *get_register_mut(&input[ip].register, &mut registers) =
                    get_register(&input[ip].register_or_number, &registers)
            }
            "sub" => {
                *get_register_mut(&input[ip].register, &mut registers) -=
                    get_register(&input[ip].register_or_number, &registers)
            }
            "mul" => {
                *get_register_mut(&input[ip].register, &mut registers) *=
                    get_register(&input[ip].register_or_number, &registers);
                mul_count += 1;
            }
            "jnz" => {
                if get_register(&input[ip].register, &registers) != 0 {
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

    return mul_count;
}

// set b 81          [1, 81, 0, 0, 0, 0, 0, 0]
// set c b           [1, 81, 81, 0, 0, 0, 0, 0]
// jnz a 2
// jnz 1 5
// mul b 100         [1, 8100, 81, 0, 0, 0, 0, 0]
// sub b -100000     [1, 108100, 81, 0, 0, 0, 0, 0]
// set c b           [1, 108100, 108100, 0, 0, 0, 0, 0]
// sub c -17000      [1, 108100, 125100, 0, 0, 0, 0, 0]
//      set f 1           [1, 108100, 125100, 0, 0, 1, 0, 0]
//      set d 2           [1, 108100, 125100, 2, 0, 1, 0, 0]
//          set e 2           [1, 108100, 125100, 2, 2, 1, 0, 0]
//              set g d           [1, 108100, 125100, 2, 2, 1, 2, 0]
//              mul g e           [1, 108100, 125100, 2, 2, 1, 4, 0]
//              sub g b           [1, 108100, 125100, 2, 2, 1, -108096, 0]
//              jnz g 2
//                  set f 0
//              sub e -1          [1, 108100, 125100, 2, 3, 1, -108096, 0]
//              set g e           [1, 108100, 125100, 2, 3, 1, 3, 0]
//              sub g b           [1, 108100, 125100, 2, 3, 1, -108097, 0]
//              jnz g -8          exit when g = 0, before g = 108100, e = 108100 / 2
//          sub d -1
//          set g d
//          sub g b
//          jnz g -13
//      jnz f 2
//          sub h -1
//      set g b
//      sub g c
//      jnz g 2
//          jnz 1 3
//      sub b -17
//      jnz 1 -23

fn part2(_input: &InputT) -> OutputT {
    // let mut ip = 0;
    // let mut registers = vec![0i64; 8];
    // registers[0] = 1;
    // let mut mul_count = 0;

    // while ip < input.len() {
    //     // println!("LastPlayerFrequency: {}", last_sound_played);
    //     // println!("Instruction: {:?}", input[ip]);
    //     println!("Ip: {}, Registers: {:?}", ip, registers);
    //     match input[ip].opcode.as_str() {
    //         "set" => {
    //             *get_register_mut(&input[ip].register, &mut registers) =
    //                 get_register(&input[ip].register_or_number, &registers)
    //         }
    //         "sub" => {
    //             *get_register_mut(&input[ip].register, &mut registers) -=
    //                 get_register(&input[ip].register_or_number, &registers)
    //         }
    //         "mul" => {
    //             *get_register_mut(&input[ip].register, &mut registers) *=
    //                 get_register(&input[ip].register_or_number, &registers);
    //             mul_count += 1;
    //         }
    //         "jnz" => {
    //             if get_register(&input[ip].register, &registers) != 0 {
    //                 let jump = get_register(&input[ip].register_or_number, &registers) - 1;
    //                 if (ip as i64) + jump < 0 {
    //                     panic!("Error")
    //                 }
    //                 ip = ((ip as i64) + jump) as usize;
    //             }
    //         }
    //         _ => panic!("Error"),
    //     }

    //     ip += 1;
    // }

    // return mul_count;

    // let mut f;
    // let mut h = 0i64;

    // for b in (108100i64..=125100i64).step_by(17) {
    //     f = 1;
    //     for d in 2..b {
    //         for e in 2..b {
    //             if d * e == b {
    //                 f = 0;
    //             }
    //         }
    //     }
    //     if f == 0 {
    //         h += 1;
    //     }
    // }

    // return h;

    let b = 108100u64;
    let c = 125100u64;
    return (b..=c)
        .step_by(17)
        .filter(|n| !primes::is_prime(*n))
        .count() as i64;
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
