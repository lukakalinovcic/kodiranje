#include <algorithm>
#include <cstdio>
#include <vector>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

bool Ask(int x) {
  printf("%d\n", x);
  fflush(stdout);
  static char buff[16];
  CHECK(scanf("%s", buff));
  return buff[0] == 'y';
}

bool IsPrime() {
  vector<int> primes = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
  vector<int> squares = {2 * 2, 3 * 3, 5 * 5, 7 * 7};
  
  int distinct_primes = 0;
  for (int prime : primes) {
    distinct_primes += Ask(prime);
    if (distinct_primes > 1) return false; 
  }
  for (int square : squares) {
    if (Ask(square)) return false;    
  }
  return true;
}

int main() {
  if (IsPrime()) printf("prime\n");
  else printf("composite\n");
  return 0;
}
