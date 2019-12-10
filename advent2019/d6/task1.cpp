#include <cstdio>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Node {
  string name;
  Node* parent = nullptr;
  vector<Node*> children;
};

map<string, Node*> nodes;
Node* GetNode(const string& name) {
  if (nodes.count(name)) return nodes[name];
  Node* node = new Node;
  node->name = name;
  return nodes[name] = node;
}

void Rec(const Node* x, int depth, int* direct, int* indirect) {
  if (depth > 0) *direct += 1;
  if (depth > 1) *indirect += depth - 1;
  for (const Node* y : x->children) {
    Rec(y, depth + 1, direct, indirect);
  }
}

int main() {
  static char s[256];
  while (scanf("%s", s) == 1) {
    int p = 0;
    while (s[p] != ')') ++p;
    s[p] = 0;
    const string aa = string(s);
    const string bb = string(s + p + 1);
    Node* a = GetNode(aa);
    Node* b = GetNode(bb);
    a->children.push_back(b);
    b->parent = a;
  }
  int direct = 0;
  int indirect = 0;
  for (const auto& entry : nodes) {
    Node* x = entry.second;
    if (x->parent == nullptr) {
      Rec(x, 0, &direct, &indirect);
    }
  }
  printf("%d\n", direct + indirect);
  return 0;
}
