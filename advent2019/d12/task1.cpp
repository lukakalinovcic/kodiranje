#include <cmath>
#include <cstdio>
#include <vector>

using namespace std;

struct Moon {
  int x, y, z;
  int vx, vy, vz;
};

vector<Moon> Read() {
  static char s[256];
  vector<Moon> moons;
  while (fgets(s, sizeof(s), stdin)) {
    Moon moon;
    sscanf(s, "<x=%d, y=%d, z=%d>", &moon.x, &moon.y, &moon.z);
    moon.vx = moon.vy = moon.vz = 0;
    moons.push_back(moon);
  }
  return moons;
}

int sgn(int x) {
  if (x < 0) return -1;
  if (x > 0) return 1;
  return 0;
}

int main() {
  vector<Moon> moons = Read();
  for (int step = 1; step <= 1000; ++step) {
    for (Moon& a : moons) {
      for (Moon& b : moons) {
        a.vx += sgn(b.x - a.x);
        a.vy += sgn(b.y - a.y);
        a.vz += sgn(b.z - a.z);
      }
    }
    for (Moon& a : moons) {
      a.x += a.vx;
      a.y += a.vy;
      a.z += a.vz;
    }
  }
  int energy = 0;
  for (const Moon& a : moons) {
    int potential = abs(a.x) + abs(a.y) + abs(a.z);
    int kinetic = abs(a.vx) + abs(a.vy) + abs(a.vz);
    energy += potential * kinetic;
  }
  printf("%d\n", energy);
  return 0;
}
