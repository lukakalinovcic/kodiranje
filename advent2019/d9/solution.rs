fn execute(tokens: Vec<&str>, input: i64)  {
  let mut prog = std::collections::BTreeMap::new();
  for (index, s) in tokens.iter().enumerate() {
    let value: i64 = s.trim().parse().expect("Error parsing token.");
    prog.insert(index as i64, value);
  }

  let mut pc: i64 = 0;
  let mut relative_base: i64 = 0;

  loop {
    let mut value = prog[&pc];
    pc += 1;
    let op = value % 100;
    value /= 100;

    fn get_param_addr(value: &mut i64, pc: &mut i64, relative_base: i64,
                      prog: &std::collections::BTreeMap<i64, i64>) -> i64 {
      let mode = *value % 10;
      *value /= 10;
      let local_pc = *pc;
      *pc += 1;
      if mode == 0 {
        return prog[&local_pc];
      } else if mode == 1 {
        return local_pc;
      } else {
        return relative_base + prog[&local_pc];
      }
    }
    let mut param_addr = || get_param_addr(&mut value, &mut pc, relative_base, &prog);

    if op == 1 {
      let a = param_addr();
      let b = param_addr();
      let c = param_addr();
      prog.insert(c, prog[&a] + prog[&b]);  
    } else if op == 2 {
      let a = param_addr();
      let b = param_addr();
      let c = param_addr();
      prog.insert(c, prog[&a] * prog[&b]);  
    } else if op == 3 {
      let c = param_addr();
      prog.insert(c, input);
    } else if op == 4 {
      let c = param_addr();
      println!("{}", prog[&c]);
    } else if op == 5 {
      let a = param_addr();
      let b = param_addr();
      if prog[&a] != 0 {
        pc = prog[&b];
      }
    } else if op == 6 {
      let a = param_addr();
      let b = param_addr();
      if prog[&a] == 0 {
        pc = prog[&b];
      }
    } else if op == 7 {
      let a = param_addr();
      let b = param_addr();
      let c = param_addr();
      if prog[&a] < prog[&b] {
        prog.insert(c, 1 as i64);
      } else {
        prog.insert(c, 0 as i64);
      }
    } else if op == 8 {
      let a = param_addr();
      let b = param_addr();
      let c = param_addr();
      if prog[&a] == prog[&b] {
        prog.insert(c, 1 as i64);
      } else {
        prog.insert(c, 0 as i64);
      }
    } else if op == 9 {
      let a = param_addr();
      relative_base += prog[&a];
    } else {
      return;
    }
  }
}

fn main() {
  let mut line = String::new();
  std::io::stdin().read_line(&mut line).expect("Error reading line.");
  let tokens: Vec<&str> = line.split(",").collect();
  execute(tokens, 2);
}
