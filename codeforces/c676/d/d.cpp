#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

using namespace std;

int R, C;
int a[4][1000][1000];

int convert_map[128];
void InitConvertMap() {
  const char* blocks = "+-|^<>vLRUD*";
  for (int i = 0; blocks[i]; ++i) {
    convert_map[static_cast<int>(blocks[i])] = i;
  }
}
int Convert(char c) { return convert_map[static_cast<int>(c)]; }

int GetBlock(int rot, int r, int c) {
  if (r < 0 || r >= R) return Convert('*');
  if (c < 0 || c >= C) return Convert('*');
  return a[rot][r][c];
}

int rotate_map[12];
void InitRotateMap() {
  const char* src = "+-|^<>vLRUD*";
  const char* dst = "+|->^v<UDRL*";
  for (int i = 0; src[i]; ++i) {
    rotate_map[Convert(src[i])] = Convert(dst[i]);
  }
}

int Rotate(int block) {
  return rotate_map[block];
}

char door_map[12];
void InitDoorMap() {
  //                         LDRU
  door_map[Convert('+')] = 0b1111;
  door_map[Convert('|')] = 0b0101;
  door_map[Convert('-')] = 0b1010;
  door_map[Convert('>')] = 0b0010;
  door_map[Convert('^')] = 0b0001;
  door_map[Convert('v')] = 0b0100;
  door_map[Convert('<')] = 0b1000;
  door_map[Convert('U')] = 0b1110;
  door_map[Convert('D')] = 0b1011;
  door_map[Convert('R')] = 0b1101;
  door_map[Convert('L')] = 0b0111;
  door_map[Convert('*')] = 0b0000;
}

const int dr[4] = { -1, 0, 1, 0 };
const int dc[4] = { 0, 1, 0, -1 }; 
bool Door(int block, int dir) {
  return (door_map[block] >> dir) & 1;
}

int dist[4][1000][1000];
struct State {
  int rot, r, c;
  int Block() { return GetBlock(rot, r, c); }
  int& Dist() { return dist[rot][r][c]; }
};

int Solve(int r1, int c1, int r2, int c2) {
  memset(dist, 0x3f, sizeof dist);
  queue<State> Q;
  dist[0][r1][c1] = 0;
  Q.push(State{0, r1, c1});
  while (!Q.empty()) {
    State u = Q.front();
    Q.pop();
    if (u.r == r2 && u.c == c2) return u.Dist();
    
    for (int d = 0; d < 4; ++d) {
      State v = {u.rot, u.r + dr[d], u.c + dc[d]};
      if (!Door(u.Block(), d) || !Door(v.Block(), d ^ 2)) continue;
      if (v.Dist() <= u.Dist() + 1) continue;
      v.Dist() = u.Dist() + 1;
      Q.push(v);
    }
    
    State v = {(u.rot + 1) % 4, u.r, u.c};
    if (v.Dist() <= u.Dist() + 1) continue;
    v.Dist() = u.Dist() + 1;
    Q.push(v);
  }
  return -1;
}

int main() {
  InitConvertMap();
  InitRotateMap();
  InitDoorMap();

  CHECK(scanf("%d%d", &R, &C));
  for (int r = 0; r < R; ++r) {
    static char buff[1001];
    CHECK(scanf("%s", buff));
    for (int c = 0; c < C; ++c) {
      a[0][r][c] = Convert(buff[c]);
      for (int i = 1; i < 4; ++i) {
        a[i][r][c] = Rotate(a[i - 1][r][c]);
      }
    }
  }

  int r1, c1;
  int r2, c2;
  CHECK(scanf("%d%d", &r1, &c1));
  CHECK(scanf("%d%d", &r2, &c2));
  printf("%d\n", Solve(r1 - 1, c1 - 1, r2 - 1, c2 - 1));
  
  return 0;
}
