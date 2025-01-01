use queues::*;
use std::{
    collections::{BTreeMap, HashMap},
    io::{self, BufRead},
};

#[derive(Debug, Clone, PartialEq, Eq, Copy, Hash, PartialOrd, Ord)]
enum Pulse {
    Low,
    High,
}

#[derive(Debug, PartialEq)]
enum Module {
    FlipFlop(String),
    Conjuntion(String),
    Broadcaster(String),
}

#[derive(Debug)]
struct Input {
    module: Module,
    destinations: Vec<String>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
struct InputModuleState {
    module_name: String,
    state: Pulse,
}

#[derive(Debug, Clone, Eq, PartialEq, PartialOrd, Ord)]
struct State {
    conjunction: BTreeMap<String, Vec<InputModuleState>>,
    flip_flop: BTreeMap<String, Pulse>,
}

type InputT = (Vec<Input>, State);
type OutputT = usize;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let inputs: Vec<Input> = stdin
        .lock()
        .lines()
        .map(|line| {
            let elem: Vec<String> = line
                .unwrap()
                .as_str()
                .split(" -> ")
                .map(|s| String::from(s))
                .collect();
            assert_eq!(elem.len(), 2);
            let destinations: Vec<String> = elem[1].split(", ").map(|s| String::from(s)).collect();
            let module = match elem[0].chars().next().unwrap() {
                'b' => Module::Broadcaster(elem[0].to_string()),
                '%' => Module::FlipFlop(elem[0][1..].to_string()),
                '&' => Module::Conjuntion(elem[0][1..].to_string()),
                _ => panic!("Error"),
            };

            Input {
                module: module,
                destinations: destinations,
            }
        })
        .collect();

    let conjunctions: Vec<String> = inputs
        .iter()
        .filter(|input| match input.module {
            Module::Conjuntion(_) => true,
            _ => false,
        })
        .map(|input| match &input.module {
            Module::Conjuntion(s) => s.clone(),
            _ => panic!("error"),
        })
        .collect();
    let mut state = State {
        conjunction: BTreeMap::new(),
        flip_flop: BTreeMap::new(),
    };
    inputs.iter().for_each(|input| {
        input
            .destinations
            .iter()
            .filter(|name| conjunctions.contains(name))
            .for_each(|i| {
                state
                    .conjunction
                    .entry(i.to_string())
                    .or_insert(Vec::new())
                    .push(InputModuleState {
                        module_name: match &input.module {
                            Module::Broadcaster(s) => s.clone(),
                            Module::FlipFlop(s) => s.clone(),
                            Module::Conjuntion(s) => s.clone(),
                        },
                        state: Pulse::Low,
                    });
            });
    });
    inputs.iter().for_each(|input| match &input.module {
        Module::FlipFlop(s) => _ = state.flip_flop.insert(s.clone(), Pulse::Low),
        _ => (),
    });

    (inputs, state)
}

fn handle_button_press<F: Fn(&State, &(String, Pulse, String)) -> bool>(
    input: &InputT,
    state: &mut State,
    f: F,
) -> (HashMap<Pulse, usize>, bool) {
    let mut signals: HashMap<Pulse, usize> = HashMap::from([(Pulse::High, 0), (Pulse::Low, 1)]);
    let mut to_process = queue![("broadcaster".to_string(), Pulse::Low, "".to_string())];
    while to_process.size() > 0 {
        let current = to_process.remove().unwrap();
        if f(&state, &current) {
            return (signals, true);
        }
        let current_module_opt = input.0.iter().find(|m| match &m.module {
            Module::FlipFlop(s) => *s == current.0,
            Module::Broadcaster(s) => *s == current.0,
            Module::Conjuntion(s) => *s == current.0,
        });
        if current_module_opt.is_none() {
            continue;
        }
        let current_module = current_module_opt.unwrap();
        match current_module.module {
            Module::FlipFlop(_) => {
                if current.1 == Pulse::High {
                    continue;
                }
                let modules = state.flip_flop.get_mut(&current.0).unwrap();
                *modules = if *modules == Pulse::Low {
                    Pulse::High
                } else {
                    Pulse::Low
                };
                *signals.get_mut(modules).unwrap() =
                    signals.get(modules).unwrap() + current_module.destinations.len();
                let _ = current_module.destinations.iter().for_each(|d| {
                    let _ = to_process.add((d.clone(), *modules, current.0.clone()));
                });
                ()
            }
            Module::Broadcaster(_) => {
                *signals.get_mut(&current.1).unwrap() =
                    signals.get(&current.1).unwrap() + current_module.destinations.len();
                let _ = current_module.destinations.iter().for_each(|d| {
                    let _ = to_process.add((d.clone(), current.1, current.0.clone()));
                });
                ()
            }
            Module::Conjuntion(_) => {
                let modules = state.conjunction.get_mut(&current.0).unwrap();
                let module_state = modules
                    .iter_mut()
                    .find(|m| m.module_name == current.2)
                    .unwrap();
                module_state.state = current.1;

                let all_high =
                    modules.iter().filter(|m| m.state == Pulse::High).count() == modules.len();

                let signal = if all_high { Pulse::Low } else { Pulse::High };
                *signals.get_mut(&signal).unwrap() =
                    signals.get(&signal).unwrap() + current_module.destinations.len();

                let _ = current_module.destinations.iter().for_each(|d| {
                    let _ = to_process.add((d.clone(), signal, current.0.clone()));
                });
                ()
            }
        }
        if current.0 != "broadcaster" && *state == input.1 {
            break;
        }
    }
    return (signals, false);
}

fn part1_functor(_state: &State, _currenti: &(String, Pulse, String)) -> bool {
    false
}

fn part1(input: &InputT) -> OutputT {
    let mut signals_total: HashMap<Pulse, usize> =
        HashMap::from([(Pulse::High, 0), (Pulse::Low, 0)]);
    let mut state = input.1.clone();
    for _pressed in 0..1000 {
        let (signals, _) = handle_button_press(input, &mut state, &part1_functor);
        *signals_total.get_mut(&Pulse::Low).unwrap() += signals.get(&Pulse::Low).unwrap();
        *signals_total.get_mut(&Pulse::High).unwrap() += signals.get(&Pulse::High).unwrap();
    }
    signals_total.get(&Pulse::Low).unwrap() * signals_total.get(&Pulse::High).unwrap()
}

fn part2(input: &InputT) -> OutputT {
    let modules: Vec<String> = input
        .1
        .conjunction
        .get("vr")
        .unwrap()
        .iter()
        .map(|m| m.module_name.clone())
        .collect();

    modules
        .iter()
        .map(|mf| {
            let module_index = input
                .1
                .conjunction
                .get("vr")
                .unwrap()
                .iter()
                .position(|m| m.module_name == *mf)
                .unwrap();

            let mut state = input.1.clone();

            for pressed in 0.. {
                let (_, part2) = handle_button_press(
                    input,
                    &mut state,
                    |state: &State, current: &(String, Pulse, String)| {
                        if current.0 == "vr" {
                            let modules = state.conjunction.get(&current.0).unwrap();
                            if modules[module_index].state == Pulse::High {
                                return true;
                            }
                        }
                        false
                    },
                );
                if part2 {
                    return pressed + 1;
                }
            }
            0
        })
        .fold(1, |acc, elem| acc * elem)
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
