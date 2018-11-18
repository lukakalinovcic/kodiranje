#include <cstdio>
#include <cstring>

template<typename T, int N>
class ArrayWithCumSum {
public:
  ArrayWithCumSum() {
    memset(a, 0, sizeof a);
    memset(b, 0, sizeof b);
  }
  
  int Size() { return N; }
  
  void Set(int i, T value) {
    const T delta = value - a[i];
    a[i] = value;
    for (i += 1; i <= N; i += i & -i) {
      b[i] += delta;
    }
  }

  T Get(int i) {
    return a[i];
  }

  T CumSum(int i) {
    T sum = 0;
    for (i += 1; i > 0; i -= i & -i) {
      sum += b[i];
    }
    return sum;
  }

  // Note: Assumes values are non-negative.
  int CumSumLowerBound(T value) {
    int p = 0;
    while ((1 << p) <= N && b[1 << p] < value) {
      ++p;
    }
    int i = 0;
    for (--p; p >= 0; --p) {
      const int j = i + (1 << p);
      if (j <= N && b[j] < value) {
        i = j;
        value -= b[i];
      }
    }
    return i;
  }
  
private:
  T a[N];
  T b[N + 1];
};

typedef long long int64;

int main() {
  ArrayWithCumSum<int64, 10> a;
  a.Set(2, 11);
  a.Set(3, 19);
  a.Set(7, 7);
  for (int i = 0; i < a.Size(); ++i) {
    printf("%d %lld %lld\n", i, a.Get(i), a.CumSum(i));
  }
  printf("0: %d\n", a.CumSumLowerBound(0));
  printf("1: %d\n", a.CumSumLowerBound(1));
  printf("10: %d\n", a.CumSumLowerBound(10));
  printf("11: %d\n", a.CumSumLowerBound(11));
  printf("12: %d\n", a.CumSumLowerBound(12));
  printf("29: %d\n", a.CumSumLowerBound(29));
  printf("30: %d\n", a.CumSumLowerBound(30));
  printf("31: %d\n", a.CumSumLowerBound(31));
  printf("36: %d\n", a.CumSumLowerBound(36));
  printf("37: %d\n", a.CumSumLowerBound(37));
  printf("38: %d\n", a.CumSumLowerBound(38));

  return 0;
}
