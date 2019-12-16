#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <map>
#include <thread>
#include <vector>

using namespace std;

typedef long long int64;

vector<int64> Read() {
  static char s[65536];
  scanf("%s", s);
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

struct Coords {
  int x, y;
};
bool operator<(const Coords &A, const Coords &B) {
  if (A.x != B.x) return A.x < B.x;
  if (A.y != B.y) return A.y < B.y;
  return 0;
}

map<Coords, int> tile;

int64 Draw(const vector<int64>& data, Coords* ball, Coords* paddle) {
  assert(data.size() % 3 == 0);
  map<Coords, int> tile;
  int64 score = -1;
  int minx = 0, maxx = 0;
  int miny = 0, maxy = 0;
  for (int i = 0; i < (int)data.size(); i += 3) {
    if (data[i] == -1) {
      score = data[i + 2];
    } else { 
      Coords C;
      C.x = data[i];
      C.y = data[i + 1];
      tile[C] = data[i + 2];
      if (data[i + 2] == 3) *paddle = C;
      if (data[i + 2] == 4) *ball = C;
      maxx = max(maxx, C.x);
      maxy = max(maxy, C.y);
    }
  }

  printf("Score %lld\n", score);
  // for (int y = maxy; y >= miny; --y) {
  //   for (int x = minx; x <= maxx; ++x) {
  //     Coords C{x, y};
  //     int t = 0;
  //     const auto it = tile.find(C);
  //     if (it != tile.end()) {
  //       t = it->second;
  //     }
  //     printf("%d", t);
  //   }
  //   printf("\n");
  // }
  return score;
}

int64 Execute(const vector<int64>& start) {
  map<int64, int64> prog;
  for (int i = 0; i < (int)start.size(); ++i) {
    prog[i] = start[i];
  }
  int64 score = 0;
  vector<int64> buff;
  int64 pc = 0;
  int64 relative_base = 0;
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
      Coords ball, paddle;
      int next_score = Draw(buff, &ball, &paddle);
      if (next_score > score) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
      }
      score = next_score;
      int64 c = ParamAddr();
      if (ball.x < paddle.x) {
        prog[c] = -1;
      } else if (ball.x > paddle.x) {
        prog[c] = 1;
      } else {
        prog[c] = 0;
      }
    } else if (op == 4) {
      int64 c = ParamAddr();
      buff.push_back(prog[c]);
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
      Coords ball, paddle;
      score = Draw(buff, &ball, &paddle);
      return score;
    }
  }
}


int main() {
  vector<int64> prog = Read();
  prog[0] = 2;
  int64 score = Execute(prog);
  printf(":: %lld\n", score);
  return 0;
}
