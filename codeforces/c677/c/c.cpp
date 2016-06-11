#include <algorithm>
#include <cctype>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

int num(char c) {
  if (isdigit(c)) return c - '0';
  if (isupper(c)) return 10 + c - 'A';
  if (islower(c)) return 36 + c - 'a';
  if (c == '-') return 62;
  CHECK(c == '_');
  return 63;
}

char s[100001];

int main() {
  CHECK(scanf("%s", s));
  int ret = 1;
  for (char *c = s; *c; ++c) {
    int x = num(*c);
    for (int i = 0; i < 6; ++i) {
      if (((x >> i) & 1) == 0) {
        ret = (ret * 3LL) % 1000000007; 
      }
    }
  }
  printf("%d\n", ret);
  return 0;
}
