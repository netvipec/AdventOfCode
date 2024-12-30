use std::cmp::Ordering;
use std::collections::HashMap;
use std::io::{self, BufRead};

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
struct Point3D {
    x: i64,
    y: i64,
    z: i64,
}

impl std::hash::Hash for Point3D {
    fn hash<H>(&self, state: &mut H)
    where
        H: std::hash::Hasher,
    {
        state.write_i64(self.x);
        state.write_i64(self.y);
        state.write_i64(self.z);
        state.finish();
    }
}

#[derive(Debug, Clone)]
struct Particle {
    p: Point3D,
    v: Point3D,
    a: Point3D,
}

type InputT = Vec<Particle>;
type OutputT = usize;

fn read_part(part: &str) -> Point3D {
    let part_nums = &part[3..part.len() - 1];
    let mut part1_parts = part_nums.split(',');
    Point3D {
        x: part1_parts.next().unwrap().parse::<i64>().unwrap(),
        y: part1_parts.next().unwrap().parse::<i64>().unwrap(),
        z: part1_parts.next().unwrap().parse::<i64>().unwrap(),
    }
}

fn read_input() -> InputT {
    let stdin = io::stdin();
    let input: InputT = stdin
        .lock()
        .lines()
        .map(|x| {
            let line = x.unwrap();
            let parts: Vec<&str> = line.split(", ").collect();
            assert_eq!(parts.iter().count(), 3);

            Particle {
                p: read_part(parts[0]),
                v: read_part(parts[1]),
                a: read_part(parts[2]),
            }
        })
        .collect();
    return input;
}

fn add(p1: &mut Point3D, p2: &Point3D) {
    p1.x += p2.x;
    p1.y += p2.y;
    p1.z += p2.z;
}

fn update(p: &mut Particle) {
    add(&mut p.v, &p.a);
    add(&mut p.p, &p.v);
}

fn distance(p: &Particle) -> usize {
    return (p.p.x.abs() + p.p.y.abs() + p.p.z.abs()) as usize;
}

fn part1(input: &InputT) -> OutputT {
    let mut particles = input.clone();
    let mut particles_index = vec![0; particles.len()];
    for i in 0..particles.len() {
        particles_index[i] = i;
    }
    for _i in 0..10000 {
        for mut p in particles.iter_mut() {
            update(&mut p);
        }

        particles_index.sort_by(|a, b| distance(&particles[*a]).cmp(&distance(&particles[*b])));
        // println!("{:?}", &particles_index[0..10]);
    }
    return particles_index[0];
}

fn part2(input: &InputT) -> OutputT {
    let mut particles = input.clone();
    let mut particles_index = vec![0; particles.len()];
    for i in 0..particles.len() {
        particles_index[i] = i;
    }
    let mut last_inserted = Point3D { x: 0, y: 0, z: 0 };
    for _i in 0..10000 {
        for mut p in particles.iter_mut() {
            update(&mut p);
        }

        let mut set = HashMap::new();
        particles_index.sort_by(|a, b| particles[*a].p.cmp(&particles[*b].p));
        for pi in &particles_index {
            if last_inserted.cmp(&particles[*pi].p) == Ordering::Equal {
                continue;
            } else {
                match set.insert(&particles[*pi].p, *pi) {
                    Some(_x) => {
                        last_inserted = particles[*pi].p.clone();
                        set.remove(&last_inserted);
                    }
                    None => {}
                }
            }
        }
        // println!("{:?}", &particles_index.len());

        particles_index = set.iter().map(|a| *a.1).collect();
    }
    return particles_index.len();
}

fn main() {
    let input = read_input();
    // println!("{:?}", input);

    let sol1 = part1(&input);
    println!("Part1: {}", sol1);
    let sol2 = part2(&input);
    println!("Part2: {}", sol2);
}
