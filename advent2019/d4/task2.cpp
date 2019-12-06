#include <cstdio>
#include <vector>

bool Ok(int x) {
  int prev = 10;
  std::vector<int> runs;
  int run = 0;
  for (int i = 0; i < 6; ++i) {
    int curr = x % 10;
    x /= 10;
    if (prev < curr) return 0;
    if (prev != curr) {
      runs.push_back(run);
      run = 1;
    } else {
      ++run;
    }
    prev = curr;
  }
  if (run > 0) {
    runs.push_back(run);
  }
  for (int x : runs) {
    if (x == 2) return true;
  }
  return false;
}

int main() {
  int a = 128392;
  int b = 643281;
  int c = 0;
  for (int x = a; x <= b; ++x) {
    c += Ok(x);
  }
  printf("%d\n", c);
  return 0;
}
