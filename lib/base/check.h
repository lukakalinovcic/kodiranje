#ifndef LIB_BASE_CHECK_H_
#define LIB_BASE_CHECK_H_
#include <cstdio>
#include <cstdlib>

#define CHECK(condition) \
if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

#ifdef LOCAL
#define DCHECK(condition) CHECK(condition)
#else
#define DCHECK(condition)
#endif

#endif  // LIB_BASE_CHECK_H_
