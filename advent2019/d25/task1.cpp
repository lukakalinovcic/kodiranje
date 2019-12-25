#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <functional>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

typedef long long int64;

vector<int64> Read() {
  FILE *f = fopen("in", "rt");
  static char s[65536];
  assert(fgets(s, sizeof s, f));
  fclose(f);
  char* p = s;
  int64 x;
  int k;
  vector<int64> prog;
  while (sscanf(p, "%lld%n", &x, &k) == 1) {
    prog.push_back(x);
    p += k;
    assert(k <= 18);
    if (*p == ',') ++p;
  }
  return prog;
}

void Execute(const vector<int64>& start,
             std::function<char*(const char*)> callback) {
  char *input = nullptr;
  char buffer[65536];
  char *output = buffer;
  
  map<int64, int64> prog;
  int64 pc = 0;
  int64 relative_base = 0;
  for (int i = 0; i < (int)start.size(); ++i) {
    prog[i] = start[i];
  }
  for (;;) {
    int64 value = prog[pc++];
    int op = value % 100; value /= 100;
    auto ParamAddr = [&]() {
      int mode = value % 10; value /= 10;
      assert(mode == 0 || mode == 1 || mode == 2);
      if (mode == 0) {
        return prog[pc++];
      } else if (mode == 1) {
        return pc++;
      } else {
        return relative_base + prog[pc++];
      }
    };
    
    if (op == 1) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      int64 c = ParamAddr();
      prog[c] = prog[a] + prog[b];
    } else if (op == 2) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      int64 c = ParamAddr();
      prog[c] = prog[a] * prog[b];
    } else if (op == 3) {
      if (input == nullptr || !*input) {
        *output = 0;
        input = callback(buffer);
        printf("In: %s", input);
        output = buffer;
      }
      int64 c = ParamAddr();
      prog[c] = *input++;
    } else if (op == 4) {
      int64 c = ParamAddr();
      *output++ = (char)prog[c];
    } else if (op == 5) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      if (prog[a] != 0) pc = prog[b];
    } else if (op == 6) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      if (prog[a] == 0) pc = prog[b];
    } else if (op == 7) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      int64 c = ParamAddr();
      prog[c] = prog[a] < prog[b];
    } else if (op == 8) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      int64 c = ParamAddr();
      prog[c] = prog[a] == prog[b];
    } else if (op == 9) {
      int64 a = ParamAddr();
      relative_base += prog[a];
    } else {
      printf("%s\n", buffer);
      return;
    }
  }
}

const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };
const char* dname[4] = {"west", "north", "east", "south"};

string ExtractName(const char* input) {
  string name;
  for (const char* c = input; *c; ++c) {
    if (*c == '=' && *(c + 1) == '=') {
      c += 3;
      const char* e = c;
      while (*e != '=') ++e;
      name = string(c, e - c - 1);
      c = e;
    }
  }
  return name;
}

vector<string> ExtractBullets(const char* input) {
  vector<string> result;
  for (const char* c = input; *c; ++c) {
    if (*c == '\n' && *(c + 1) == '-' && *(c + 2) == ' ') {
      c += 3; 
      const char* e = c;
      while (*e != '\n') ++e;
      result.push_back(string(c, e - c));
      c = e - 1;
    }
  }
  return result;
}

set<string> ExtractDirections(const char* input) {
  set<string> dirs;
  for (const string& bullet : ExtractBullets(input)) {
    for (int d = 0; d < 4; ++d) {
      if (bullet == dname[d]) {
        dirs.insert(bullet);
      }
    }
  }
  return dirs;
}

set<string> ExtractItems(const char* input) {
  set<string> items;
  for (const string& bullet : ExtractBullets(input)) {
    bool is_dir = false;
    for (int d = 0; d < 4; ++d) {
      if (bullet == dname[d]) {
        is_dir = true;
      }
    }
    if (!is_dir) {
      items.insert(bullet);
    }
  }
  return items;
}

struct Coords {
  int x, y;
};
bool operator<(const Coords &A, const Coords &B) {
  if (A.x != B.x) return A.x < B.x;
  if (A.y != B.y) return A.y < B.y;
  return 0;
}

struct Cell {
  string name;
  set<string> dirs;
  set<string> items;
  bool visited = false;
};

class Game {
 public:
  Game(const set<string>& bad_items) : bad_items(bad_items) {
    curr = mini = maxi = Coords{0, 0};
  }

  Coords FindRoom(const string& name) {
    for (const auto& entry : state) {
      if (entry.second.name == name) return entry.first;
    }
    state[curr].name = name;
    return curr;
  }
  
  char* Callback(const char* input) {
    printf("%s\n", input);
    if (phase == "explore") {
      return Explore(input);
    } else if (phase == "brute") {
      return Brute(input);
    } else {
      return Manual(input);
    }
  }

  char* Brute(const char* input) {
    string test_name = ExtractName(input);
    if (!test_name.empty() && test_name != "Security Checkpoint") {
      exit(0);
    }

    static char output[128];
    static int mask = 1;
    static int step = 0;
    const int n = inventory.size();
    while (step < n) {
      int i = step++;
      if (!((mask >> i) & 1)) {
        sprintf(output, "drop %s\n", inventory[i].c_str());
        return output;
      }
    }
    if (step == n) {
      sprintf(output, "west\n");
      ++step;
      return output;
    }
    while (step <= 2 * n) {
      int i = step++ - n - 1;
      if (!((mask >> i) & 1)) {
        sprintf(output, "take %s\n", inventory[i].c_str());
        return output;
      }
    }

    sprintf(output, "inv\n");
    ++mask;
    if (mask == (1 << inventory.size())) {
      exit(0);
    }
    step = 0;
    return output;
  }

  char* Manual(const char* input) {
    static char output[128];
    Draw();
    assert(fgets(output, sizeof output, stdin));
    if (string(output) == "brute\n") {
      phase = "brute";
    }
    return output;
  }

  char* Explore(const char* input) {
    static char output[128];
    bool first_time = false;
    if (!state[curr].visited) {
      state[curr].visited = true;
      first_time = true;
    }
    string test_name = ExtractName(input);
    Coords room = test_name.empty() ? curr : FindRoom(test_name);
    if (room.x != curr.x || room.y != curr.y) {
      curr = trace.top().first;
      trace.pop();
      assert(curr.x == room.x && curr.y == room.y);
    }
    if (first_time) {
      state[curr].dirs = ExtractDirections(input);
      state[curr].items = ExtractItems(input);
    } 
    if (!taking.empty()) {
      state[curr].items.erase(taking);
      inventory.push_back(taking);
    }
    Draw();
    string name = state[curr].name;

    taking.clear();
    bool moving = false;
    for (const string& item : state[curr].items) {
      if (bad_items.count(item)) continue;
      sprintf(output, "take %s\n", item.c_str());
      taking = item;
      break;
    }
    for (int d = 3; d >= 0; --d) {
      if (!taking.empty() || moving) continue;
      if (state[curr].dirs.count(dname[d])) {
        Coords next{curr.x + dx[d], curr.y + dy[d]};
        if (name == "Sick Bay" && d == 3) {
          next.y -= 1;
        }
        if (name == "Arcade" && d == 1) {
          next.y += 1;
        }
        if (state[next].visited) continue;
        sprintf(output, "%s\n", dname[d]);
        trace.emplace(curr, dname[d^2]);
        curr = next;
        moving = true;
      }
    }
    if (taking.empty() && !moving) {
      if (trace.empty()) {
        phase = "manual";
        sprintf(output, "\n");
      } else {
        curr = trace.top().first;
        sprintf(output, "%s\n", trace.top().second.c_str());
        trace.pop();
      }
    }
    return output;
  }

  void Draw() {
    mini.x = min(mini.x, curr.x);
    mini.y = min(mini.y, curr.y);
    maxi.x = max(maxi.x, curr.x);
    maxi.y = max(maxi.y, curr.y);
    map<int, int> width;
    map<int, int> height;
    for (int y = maxi.y; y >= mini.y; --y) {
      for (int x = mini.x; x <= maxi.x; ++x) {
        const Cell& cell = state[Coords{x, y}];
        if (!cell.visited) continue;
        width[x] = max(width[x], (int)cell.name.size() + 4);
        for (const string& item : cell.items) {
          width[x] = max(width[x], (int)item.size() + 4);
        }
        height[y] = max(height[y], 3 + ((int)cell.items.size() + 1));
      }
    }
    map<int, int> r0;
    map<int, int> c0;
    int total_width = 0;
    int total_height = 0;
    for (int y = maxi.y; y >= mini.y; --y) {
      r0[y] = total_height;
      total_height += height[y];
    }
    for (int x = mini.x; x <= maxi.x; ++x) {
      c0[x] = total_width;
      total_width += width[x];
    }
    vector<string> a(total_height);
    for (int i = 0; i < total_height; ++i) a[i] = string(total_width, ' ');

    for (int y = maxi.y; y >= mini.y; --y) {
      for (int x = mini.x; x <= maxi.x; ++x) {
        const Cell& cell = state[Coords{x, y}];
        if (!cell.visited) continue;
        int r1 = r0[y];
        int c1 = c0[x];
        int r2 = r1 + height[y] - 1;
        int c2 = c1 + width[x] - 1;
        if (y == curr.y && x == curr.x) {
          a[r1][c1] = a[r1][c2] = a[r2][c1] = a[r2][c2] = '*';
        } else {
          a[r1][c1] = a[r1][c2] = a[r2][c1] = a[r2][c2] = '+';
        }
        if (!cell.dirs.count("north")) {
          for (int c = c1 + 1; c < c2; ++c) a[r1][c] = '-';
        }
        if (!cell.dirs.count("south")) {
          for (int c = c1 + 1; c < c2; ++c) a[r2][c] = '-';
        }
        if (!cell.dirs.count("west")) {
          for (int r = r1 + 1; r < r2; ++r) a[r][c1] = '|';
        }
        if (!cell.dirs.count("east")) {
          for (int r = r1 + 1; r < r2; ++r) a[r][c2] = '|';
        }
        int offset = (1 + width[x] - cell.name.size()) / 2;
        for (int i = 0; i < (int)cell.name.size(); ++i) {
          a[r1 + 1][c1 + offset + i] = cell.name[i];
        }
        int r = r1 + 3;
        for (const string& item : cell.items) {
          for (int i = 0; i < (int)item.size(); ++i) {
            a[r][c1 + 2 + i] = item[i];
          }
          ++r;
        }
      }
    }
    for (const string& row : a) {
      printf("%s\n", row.c_str());
    }
  }

  string phase;
  string taking;
  Coords curr;
  Coords mini, maxi;
  map<Coords, Cell> state;
  stack<pair<Coords, string>> trace;
  vector<string> inventory;
  const set<string>& bad_items;
};

int main() {
  vector<int64> prog = Read();
  set<string> bad_items = {"infinite loop", "giant electromagnet"};
  for (;;) {
    Game game(bad_items);
    game.phase = "explore";
    Execute(prog, std::bind(&Game::Callback, &game, std::placeholders::_1));
    if (!game.taking.empty()) {
      printf("%s is bad!\n", game.taking.c_str());
      bad_items.insert(game.taking);
    }
    if (game.phase != "explore") break;
  }
  return 0;
}
