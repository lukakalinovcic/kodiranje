#include <cstdio>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Node {
  string name;
  int depth;
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

void Rec(Node* x, int depth) {
  x->depth = depth;
  for (Node* y : x->children) {
    Rec(y, depth + 1);
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
  for (const auto& entry : nodes) {
    Node* x = entry.second;
    if (x->parent == nullptr) {
      Rec(x, 0);
    }
  }
  Node* a = GetNode("YOU");
  Node* b = GetNode("SAN");
  int result = 0;
  a = a->parent;
  b = b->parent;
  for (; a->depth > b->depth; a = a->parent) ++result; 
  for (; b->depth > a->depth; b = b->parent) ++result;
  while (a != b) {
    a = a->parent;
    b = b->parent;
    result += 2;
  }
  printf("%d\n", result);
  return 0;
}
