use queues::*;
use std::{
    collections::HashMap,
    io::{self, Read},
};
use substring::Substring;

#[derive(Debug, PartialEq, Clone)]
enum Operator {
    None,
    LessThan,
    GreaterThan,
}

#[derive(Debug, Clone)]
struct WorkFlow {
    variable: String,
    operator: Operator,
    value: i64,
    to_workflow: String,
}

#[derive(Debug)]
struct WorkFlows {
    name: String,
    workflows: Vec<WorkFlow>,
}

#[derive(Debug, Clone)]
struct Ratings {
    variables: HashMap<String, i64>,
}

#[derive(Debug)]
struct Input {
    workflows: Vec<WorkFlows>,
    ratings: Vec<Ratings>,
}

type InputT = Input;
type OutputT = i64;

fn read_input() -> InputT {
    let stdin = io::stdin();
    let mut buffer = String::new();
    let _ = stdin.lock().read_to_string(&mut buffer);

    let parts: Vec<_> = buffer.as_str().split("\n\n").collect();

    let input = Input {
        workflows: parts[0]
            .split("\n")
            .map(|line| {
                let start = line.find("{").unwrap();
                let end = line.find("}").unwrap();
                WorkFlows {
                    name: line[..start].to_string(),
                    workflows: line[start + 1..end]
                        .split(",")
                        .map(|workflow| match workflow.find(":") {
                            None => WorkFlow {
                                variable: "".to_string(),
                                operator: Operator::None,
                                value: 0,
                                to_workflow: workflow.to_string(),
                            },
                            Some(index) => {
                                let i = if workflow[..index].find("<").is_some() {
                                    workflow[..index].find("<").unwrap()
                                } else {
                                    workflow[..index].find(">").unwrap()
                                };
                                let operator = match workflow[..index].find("<") {
                                    Some(_) => Operator::LessThan,
                                    None => Operator::GreaterThan,
                                };
                                WorkFlow {
                                    variable: workflow[..i].to_string(),
                                    operator: operator,
                                    value: workflow[i + 1..index].parse::<i64>().unwrap(),
                                    to_workflow: workflow[index + 1..].to_string(),
                                }
                            }
                        })
                        .collect(),
                }
            })
            .collect(),
        ratings: parts[1]
            .split("\n")
            .map(|line| Ratings {
                variables: line
                    .substring(1, line.len() - 1)
                    .split(",")
                    .map(|variable| {
                        let mut variable_part = variable.split("=");
                        (
                            variable_part.next().unwrap().to_string(),
                            variable_part.next().unwrap().parse::<i64>().unwrap(),
                        )
                    })
                    .collect(),
            })
            .collect(),
    };
    return input;
}

fn part1(input: &InputT) -> OutputT {
    let empty_rating = Ratings {
        variables: HashMap::from([
            ("a".to_string(), 0),
            ("x".to_string(), 0),
            ("s".to_string(), 0),
            ("m".to_string(), 0),
        ]),
    };
    input
        .ratings
        .iter()
        .filter(|rating| {
            let mut workflow_name = "in".to_string();
            while workflow_name != "A" && workflow_name != "R" {
                let workflow_to_apply = input
                    .workflows
                    .iter()
                    .find(|workflow| workflow.name == workflow_name)
                    .unwrap();
                for subworkflow in workflow_to_apply.workflows.iter() {
                    if subworkflow.operator == Operator::None
                        || (subworkflow.operator == Operator::LessThan
                            && *rating.variables.get(&subworkflow.variable).unwrap()
                                < subworkflow.value)
                        || (subworkflow.operator == Operator::GreaterThan
                            && *rating.variables.get(&subworkflow.variable).unwrap()
                                > subworkflow.value)
                    {
                        workflow_name = subworkflow.to_workflow.clone();
                        break;
                    }
                }
            }
            workflow_name == "A"
        })
        .fold(empty_rating, |base, elem| Ratings {
            variables: HashMap::from([
                (
                    "a".to_string(),
                    base.variables.get("a").unwrap() + elem.variables.get("a").unwrap(),
                ),
                (
                    "x".to_string(),
                    base.variables.get("x").unwrap() + elem.variables.get("x").unwrap(),
                ),
                (
                    "s".to_string(),
                    base.variables.get("s").unwrap() + elem.variables.get("s").unwrap(),
                ),
                (
                    "m".to_string(),
                    base.variables.get("m").unwrap() + elem.variables.get("m").unwrap(),
                ),
            ]),
        })
        .variables
        .into_iter()
        .fold(0i64, |acc, rating| acc + rating.1)
}

fn get_range(
    base_range: &HashMap<String, (i64, i64)>,
    workflow: &WorkFlow,
) -> HashMap<String, (i64, i64)> {
    let mut new_range = base_range.clone();
    if workflow.operator == Operator::None {
        new_range
    } else if workflow.operator == Operator::LessThan {
        let variable_value = new_range.get_mut(&workflow.variable).unwrap();
        if variable_value.0 > workflow.value {
            HashMap::from([
                ("a".to_string(), (1, 1)),
                ("x".to_string(), (1, 1)),
                ("s".to_string(), (1, 1)),
                ("m".to_string(), (1, 1)),
            ])
        } else if variable_value.1 < workflow.value {
            new_range
        } else {
            variable_value.1 = workflow.value;
            new_range
        }
    } else {
        let variable_value = new_range.get_mut(&workflow.variable).unwrap();
        if variable_value.1 < workflow.value {
            HashMap::from([
                ("a".to_string(), (1, 1)),
                ("x".to_string(), (1, 1)),
                ("s".to_string(), (1, 1)),
                ("m".to_string(), (1, 1)),
            ])
        } else if variable_value.0 > workflow.value {
            new_range
        } else {
            variable_value.0 = workflow.value + 1;
            new_range
        }
    }
}

fn part2(input: &InputT) -> OutputT {
    let rating_ranges = HashMap::from([
        ("a".to_string(), (1i64, 4001i64)),
        ("x".to_string(), (1i64, 4001i64)),
        ("s".to_string(), (1i64, 4001i64)),
        ("m".to_string(), (1i64, 4001i64)),
    ]);
    let in_index = input
        .workflows
        .iter()
        .position(|workflow| workflow.name == "in".to_string())
        .unwrap();

    let mut solution = Vec::new();
    let mut to_process = queue![(in_index, rating_ranges)];
    while to_process.size() > 0 {
        let current = to_process.remove().unwrap();
        let mut range = current.1;

        for workflow in input.workflows[current.0].workflows.iter() {
            if workflow.to_workflow == "A" {
                solution.push(get_range(&range, workflow));
            } else if workflow.to_workflow == "R" {
            } else {
                let out_index = input
                    .workflows
                    .iter()
                    .position(|w| w.name == workflow.to_workflow)
                    .unwrap();
                _ = to_process.add((out_index, get_range(&range, workflow)));
            }
            if workflow.operator == Operator::None {
                continue;
            }
            let mut new_workflow = workflow.clone();
            new_workflow.operator = if workflow.operator == Operator::LessThan {
                Operator::GreaterThan
            } else {
                Operator::LessThan
            };
            new_workflow.value = if workflow.operator == Operator::LessThan {
                new_workflow.value - 1
            } else {
                new_workflow.value + 1
            };
            range = get_range(&range, &new_workflow);
        }
    }

    solution.iter().fold(0i64, |acc, elem| {
        acc + elem
            .iter()
            .fold(1i64, |acc1, elem1| acc1 * (elem1.1 .1 - elem1.1 .0))
    })
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
