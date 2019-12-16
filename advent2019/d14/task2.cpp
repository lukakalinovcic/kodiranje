#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <set>
#include <map>
#include <utility>
#include <string>
#include <vector>

using namespace std;

struct Unit {
  int amount;
  string name;
};

struct Reaction {
  vector<Unit> input;
  Unit output;
};

map<string, Reaction> reactions;

void Input() {
  char s[256];
  while (fgets(s, sizeof(s), stdin)) {
    Reaction curr;
    static char name[128];
    int amount, k;
    char *p = s;
    while (sscanf(p, "%d%s%n", &amount, name, &k) == 2) {
      int l = strlen(name);
      if (name[l - 1] == ',') name[l - 1] = 0;
      
      curr.input.push_back(Unit{amount, name});
      p += k;
    }
    p += 4;
    assert(sscanf(p, "%d%s", &amount, name) == 2);
    curr.output = Unit{amount, name};
    reactions[name] = curr;
  }
}

void dfs(const string& name, set<string>* seen, vector<string>* topo) {
  seen->insert(name);
  if (reactions.count(name)) {
    for (const Unit& u : reactions[name].input) {
      if (!seen->count(u.name)) {
        dfs(u.name, seen, topo);
      }
    }
  }
  topo->push_back(name);
}

typedef long long int64;

int64 RequiredOre(int64 required_fuel, const vector<string>& topo) {
  map<string, int64> required;
  required["FUEL"] = required_fuel;
  for (const string& name : topo) {
    if (!reactions.count(name)) {
      continue;
    }
    const Reaction& r = reactions[name];
    int64 k = (required[name] + r.output.amount - 1) / r.output.amount;
    for (const Unit& u : r.input) {
      required[u.name] += u.amount * k;
    }
  }
  return required["ORE"];
}

int main(void) {
  Input();
  vector<string> topo;
  set<string> seen;
  dfs("FUEL", &seen, &topo);
  reverse(topo.begin(), topo.end());

  int lo = 1, hi = 10000000;
  while (lo < hi) {
    int mid = (lo + hi + 1) / 2;
    int64 ore = RequiredOre(mid, topo);
    if (ore > 1000000000000LL) {
      hi = mid - 1;
    } else {
      lo = mid;
    }
  }
  printf("%d\n", lo);
  return 0;
}
