# Range statistic array
Implementira tournament stablo nad nizom, i razne operacije nad stablom. Krenimo redom.
## Elementi niza i statistika intervala
Minimalno sto trebamo je definirati kojeg tipa su elementi niza, te kojeg tipa je statistika koju pamtimo za intervale niza. Elementi niza mogu biti proizvoljnog tipa, dok statistika mora biti struktura koja ima dvije odredjene staticke funkcije.
Uzmimo za primjer da su vrijednosti int, a zanima nas sum, min i max. Tada definiramo statistiku:
```C++
struct Stats {
  long long sum;
  int min;
  int max;
  static void FromValue(const range_statistic::ArrayRange& range, int value,
                        Stats* stats) {
    stats->sum = value;
    stats->min = value;
    stats->max = value;
  }
  static void Merge(const range_statistic::ArrayRange& range,
                    const Stats& left, const Stats& right, Stats* stats) {
    stats->sum = left.sum + right.sum;
    stats->min = std::min(left.min, right.min);
    stats->max = std::max(left.max, right.max);
  }
};

int main() {
  range_statistic::Array<int, Stats> arr(vector<int>{5, 4, 3, 1, 7});
  printf("arr[3] = %d\n", arr.Get(3));
  printf("sum: %lld\n", arr.GetStats(0, 5).sum);
  *arr.GetMutable(3) = 8;
  printf("sum: %lld\n", arr.GetStats(0, 5).sum);
  return 0;
}
```
```
Output:
arr[3] = 1
sum: 20
sum: 27
```

`range_statistic::ArrayRange` je jednostavna struktura koja ima membere `lo` i `hi` kako bi znali za koji dio niza racunamo statistiku. U ovom slucaju statistika ne ovisi o indeksu u nizu.

Vidimo da niz ima metode:
* `Get(int index)` koja vraca trenutnu vrijednost niza,
* `GetStats(int begin, int end)` koje vracaju statistiku za interval [begin, end>, te
* `GetMutable(int index)` koji vraca objekt kojim mozemo modificirati niz. Objekt prilikom destrukcije preracunava statistiku na putu od elementa kojeg smo modificirali do vrha stabla.

## Propagacija
Ukoliko zelimo radit modifikacije na intervalima, moramo definirati strukturu podataka koja cuva podatke za propagaciju. Struktura mora imati cetiri odredjene metode. Uzmimo za primjer da zelimo dodati konstantu svim brojevima u intervalu.
```C++
struct Translation {
  int delta;
  void Clear() { delta = 0; }
  void Add(const Translation& other) { delta += other.delta; }
  void PropagateStats(const range_statistic::ArrayRange& range, Stats* stats)
      const {
    stats->sum += (long long)range.size() * delta;
    stats->min += delta;
    stats->max += delta;
  }
  void PropagateValue(const range_statistic::ArrayRange& range, int* value)
      const {
    *value += delta;
  }
};

int main() {
  range_statistic::Array<int, Stats, Translation> arr(vector<int>{5, 4, 3, 1, 7});
  printf("min: %d  max: %d\n", arr.GetStats(0, 5).min, arr.GetStats(0, 5).max);
  arr.Update(1, 4, Translation{4});
  printf("min: %d  max: %d\n", arr.GetStats(0, 5).min, arr.GetStats(0, 5).max);
  return 0;
}
```
```
Output:
min: 1  max: 7
min: 5  max: 8
```

## Obilazak stabla

Konacno, stablo pruza nacin za implementaciju custom obilazaka. Potrebno je definirati strukturu koja opet ima cetiri odredjene metode:
* `GoUp(const range_statistic::ArrayRange& range, const StatsType& stats)` koja se poziva kad prvi put posjetimo cvor. Ukoliko funkcija vrati true, prekidamo s podstablom i vracamo se prema roditelju. U protivnome:
* ukoliko se nalazimo u listu stabla, pozvat ce se funkcija `Leaf(const range_statistic::ArrayRange& range, const ValueType& value)`,
* inace, poziva se funkcija `GoLeftFirst(const range_statistic::ArrayRange& range, const StatsType& stats)` koja odlucuje hoce li se djeca pozvati u redoslijedu "lijevo pa desno" ili "desno pa lijevo".
* Konacno funkcija `IsDone()` se poziva izmedju nakon povratka iz prvog djeteta. Ukoliko vrati true, obilazak nastavljamo povratkom u roditelja, umjesto u drugo dijete.

Na dva primjera prikazat cemo kako implementirati obilazak stabla. U prvom primjeru zelimo ispisati elemente niza na indexima [lo, hi> u obrnutom poretku u O(log n + k):

```C++
struct OutputRangeReversedOp {
  int lo, hi;
  bool done;
  OutputRangeReversedOp(int lo, int hi) : lo(lo), hi(hi), done(false) {}

  bool IsDone() { return done; }
  bool GoUp(const range_statistic::ArrayRange& range, const Stats& stats) {
    if (range.hi <= lo) {
      done = true;
      return true;
    }
    return range.lo >= hi;
  }
  bool GoLeftFirst(const range_statistic::ArrayRange& range,
                   const Stats& stats) {
    return false;
  }
  void Leaf(const range_statistic::ArrayRange& range, int value) {
    printf("%d ", value);
  }
};

int main() {
  range_statistic::Array<int, Stats, Translation> arr(vector<int>{5, 4, 3, 1, 7});
  OutputRangeReversedOp op(1, 4);
  arr.Walk(op);
  return 0;
}
```
```
Output:
1 3 4 
```

U drugom primjeru iskoristit cemo obilazak stabla da bismo u O(log n) pronasli lower_bound u nizu koji bi dobili kad bi izracinuali parcijalnu sumu naseg niza `arr`.

```C++
struct PrefixSumLowerBoundOp {
  int query;
  int result;
  bool done;
  PrefixSumLowerBoundOp(int query) : query(query), result(0), done(false) {}

  bool IsDone() { return done; }
  bool GoUp(const range_statistic::ArrayRange& range, const Stats& stats) {
    if (stats.sum < query) {
      query -= stats.sum;
      result = range.hi;
      return true;
    }
    return false;
  }
  bool GoLeftFirst(const range_statistic::ArrayRange& range,
                   const Stats& stats) {
    return true;
  }
  void Leaf(const range_statistic::ArrayRange& range, int value) {
    done = true;
  }
};

int main() {
  range_statistic::Array<int, Stats, Translation> arr(vector<int>{2, 3, 1, 2});
  for (int i = 0; i <= 10; ++i) {
    PrefixSumLowerBoundOp op(i);
    arr.Walk(op);
    printf("%d ", op.result);
  }
  return 0;
}
```
```
Output:
0 0 0 1 1 1 2 3 3 4 4
```

