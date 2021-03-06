#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

#define EPS 1e-9
#define eq(a,b) (((a)-EPS <= (b)) && ((b) <= (a)+EPS))
#define lt(a,b) ((a)+EPS < (b))
#define gt(a,b) ((a)-EPS > (b))
#define lte(a,b) ((a)-EPS <= (b))
#define gte(a,b) ((a)+EPS >= (b))
#define sq(a) ((a)*(a))

struct point {
  double x, y; 
  point() { x = y = 0; }
  point(const point &drugi) { *this = drugi; }
  point(double X, double Y) { x = X; y = Y; }
};

struct segment {
  point a, b;
  segment() {}
  segment(const segment &drugi) { *this = drugi; }
  segment(const point &A, const point &B) { a = A; b = B; }
  segment(double X1, double Y1, double X2, double Y2) {
    a = point(X1, Y1);
    b = point(X2, Y2);
  }
};

struct line { // !! Ax+By=C, a ne Ax+By+C=0 !!
  double A, B, C;
  line() { A = B = C = 0; }  
  line(const line &drugi) { *this = drugi; }
  line(double a, double b, double c) { A = a; B = b; C = c; }
  line(const segment &S) { 
    A = S.b.y - S.a.y; 
    B = S.a.x - S.b.x;
    C = A*S.a.x + B*S.a.y;
  }
  line(const point &A, const point &B) { *this = line(segment(A, B)); }
  line(double fi, const point &T) {
    A = sin(fi);
    B = -cos(fi);
    C = A*T.x + B*T.y;
  }
};

/*******************************************************/
int mysgn(double val) {
  if (lt(val, 0)) return -1; 
  if (gt(val, 0)) return 1;
  return 0;
}       

double myhypot(double A, double B) {
  return sqrt(A*A + B*B);
}

pair<double,double> kvadratna(double A, double B, double C) {
  if (eq(sq(B)-4*A*C, 0)) return make_pair(-B/(2*A),-B/(2*A));
  return make_pair((-B+sqrt(sq(B)-4*A*C))/(2*A),
                   (-B-sqrt(sq(B)-4*A*C))/(2*A));
}

int strana(const line &L, const point &T) {
  return mysgn(L.A*T.x + L.B*T.y - L.C);
}

int ccw(const point &A, const point &B, const point &C) {
  return mysgn((B.x-A.x)*(C.y-A.y) - (C.x-A.x)*(B.y-A.y));
}

line okomica(const line &L, const point &T) {
  line ret; 
  ret.A = L.B; ret.B = -L.A;
  ret.C = ret.A*T.x + ret.B*T.y;
  return ret;
}

point midpoint(const point &A, const point &B) {
  return point((A.x+B.x)/2, (A.y+B.y)/2);
}

/*******************************************************/
bool intersects(const line &L, const point &P) {
  return eq(L.A*P.x + L.B*P.y - L.C, 0);
}

bool intersects(const segment &S, const point &P) {
  return intersects(line(S), P) && 
      eq(fabs(S.b.x-P.x)+fabs(S.a.x-P.x), fabs(S.b.x-S.a.x)) &&
      eq(fabs(S.b.y-P.y)+fabs(S.a.y-P.y), fabs(S.b.y-S.a.y));
}

bool intersects(const line &L1, const line &L2) {
  return !eq(L1.A*L2.B, L2.A*L1.B);
}

bool intersects(const line &L, const segment &S) {
  return strana(L, S.a) * strana(L, S.b) <= 0;
}

bool intersects(const segment &S1, const segment &S2) {
   if (intersects(line(S1), S2.a) && intersects(line(S1), S2.b))
      return intersects(S1, S2.a) || intersects(S1, S2.b) ||
             intersects(S2, S1.a) || intersects(S2, S1.b);
   return intersects(line(S1), S2) && intersects(line(S2), S1);  
}

point intersect(const line &L1, const line &L2) {
   return point((L1.C*L2.B-L2.C*L1.B) / (L1.A*L2.B-L2.A*L1.B),
                 (L1.A*L2.C-L2.A*L1.C) / (L1.A*L2.B-L2.A*L1.B));
}

point intersect(const line &L, const segment &S) {
  return intersect(L, line(S));
}

point intersect(const segment &S1, const segment &S2) {
  return intersect(line(S1), line(S2));
}

/*******************************************************/
double dist(const point &A, const point &B) {
  return myhypot(A.x-B.x, A.y-B.y);
}

double dist(const line &L, const point &T) {
  return fabs(L.A*T.x + L.B*T.y - L.C) / myhypot(L.A, L.B);
}

double dist(const segment &S, const point &T) {
  double a = dist(S.b, T), b = dist(S.a, T), c = dist(S.a, S.b);
  if (gte(sq(b), sq(a)+sq(c))) return a;
  if (gte(sq(a), sq(b)+sq(c))) return b;
  return dist(line(S), T);
}
double dist(const segment &S1, const segment &S2) {
  if (intersects(S1, S2)) return 0;
  return min(min(dist(S1,S2.a), dist(S1,S2.b)),
             min(dist(S2,S1.a), dist(S2,S1.b)));
}

double dist(const line &L1, const line &L2) {
  if (intersects(L1, L2)) return 0;
  return dist(intersect(L1, okomica(L2,point())),
              intersect(L2, okomica(L1,point())));
}

double dist(const line &L, const segment &S) {
  if (intersects(L, S)) return 0;
  return min(dist(L, S.a), dist(L, S.b));
}
/*******************************************************/
struct circle {
  point S;
  double r;
  circle() { S.x = S.y = r = 0; }
  circle(const circle &drugi) { *this = drugi; }
  circle(double X, double Y, double R) { S.x = X; S.y = Y; r = R; }
  circle(const point &T, double R) { S = T; r = R; };   
  circle(const point &A, const point &B, const point &C) {
    S = intersect(okomica(line(A,B), midpoint(A,B)),
                  okomica(line(B,C), midpoint(B,C)));
    r = dist(S, A);
  }
};

bool intersects(const circle &C, const line &L) {
  return lte(dist(L, C.S), C.r);
}

bool intersects(const circle &C, const segment &S) {
  return lte(dist(S, C.S), C.r);
}

bool intersects(const circle &C1, const circle &C2) {
  double d = dist(C1.S, C2.S);
  return !(lt(d, fabs(C1.r-C2.r)) || gt(d, C1.r + C2.r) || 
           (eq(d, 0) && eq(C1.r, C2.r)));
}

pair<point,point> intersect(circle C, line L) {
  pair<double,double> X, Y; 
  bool swp = eq(L.B, 0);
  if (swp) swap(L.A, L.B), swap(C.S.x, C.S.y);
  X = kvadratna(sq(L.A)/sq(L.B) + 1,
                2*(L.A*C.S.y/L.B - L.A*L.C/sq(L.B) - C.S.x),
                sq(C.S.x) + sq(C.S.y) - sq(C.r) + sq(L.C)/sq(L.B) - 2*L.C*C.S.y/L.B);
  Y.first  = (L.C - L.A*X.first)/L.B;
  Y.second = (L.C - L.A*X.second)/L.B;
  if (swp) swap(X, Y);
  return make_pair(point(X.first, Y.first), point(X.second, Y.second));
}

pair<point,point> intersect(const circle &C1, const circle &C2) {
  double d = dist(C1.S, C2.S);
  double a = (sq(C1.r) - sq(C2.r) + sq(d)) / (2*d);
  point T(C1.S.x+a*(C2.S.x-C1.S.x)/d, 
          C1.S.y+a*(C2.S.y-C1.S.y)/d);
  return intersect(C1, okomica(line(C1.S,C2.S), T));
}

pair<point,point> tangent_points(const circle &C, const point &P) {
  return intersect(C, circle(midpoint(P,C.S), dist(P,C.S)/2));
}

pair<line,line> tangents(const circle &C, const point &P) {
  pair<point,point> Q = tangent_points(C, P);
  return make_pair(line(P, Q.first), line(P, Q.second));
}

/*******************************************************/
struct convex_polygon {
  struct ccwcmp {
    point O;
    ccwcmp(const point &p) { O = p; }         
    bool operator () (const point &a, const point &b) {
      return ccw(O, a, b) >= 0;
    }
  };
  
  vector<point> p;
  convex_polygon() {}
  convex_polygon(const convex_polygon &drugi) { p = drugi.p; }
  convex_polygon(const vector<point> &x) {
    p = x;
    for (int i = 1; i < (int)p.size(); ++i) {
      if (p[i].y < p[0].y || (p[i].y == p[0].y && p[i].x < p[0].x)) {
        swap(p[0], p[i]);
      }
    }
    sort(p.begin()+1, p.end(), ccwcmp(p[0]));
    
    int n = 2;
    for (int i = 2; i < (int)p.size(); ++i) {         
      p[n++] = p[i];
      if (ccw(p[0], p[n-2], p[n-1]) == 0 && // kolinearni
          dist(p[0],p[n-1]) > dist(p[0],p[n-2])) {
        p[n-2] = p[n-1];
        --n;
      }
      while(ccw(p[n-3], p[n-2], p[n-1]) < 0) {
        p[n-2] = p[n-1];
        --n;
      }
    }
    p.resize(n);
  }
  inline int size() const { return p.size(); }
  inline point & operator [] (int i) { return p[i]; }
};

int main(void) {
  segment S1(3, 2, 0, 5);
  segment S2(0, 0, 5, 1);
  cout << intersects(S1, S2) << endl;
  cout << dist(S1, S2) << endl;
  line L1 = line(segment(0, 1, 1, 2));
  line L2 = line(segment(0, 0, 1, 1));
  cout << intersects(L1, L2) << endl;
  cout << dist(L1, L2) << endl;
  cout << ccw(point(0, 0), point(1, 0), point(1, 1)) << endl;
  
  circle C(5, 5, 2);
  pair<point,point> P = intersect(C, line(segment(0, 0, 1, 1)));
  cout << P.first.x << " " << P.first.y << endl;
  cout << P.second.x << " " << P.second.y << endl;
  P = intersect(C, line(segment(5, 3, 6, 4)));
  cout << P.first.x << " " << P.first.y << endl;
  cout << P.second.x << " " << P.second.y << endl;
  P = intersect(C, line(segment(3, 5, 3, 6)));
  cout << P.first.x << " " << P.first.y << endl;
  cout << P.second.x << " " << P.second.y << endl;
  segment S3(0, 0, 2, 2);
  segment S4(3, 3, 4, 4);
  cout << dist(S3, S4) << endl;
  circle C2(point(7,-5), point(-1,1), point(0,-6));
  cout << C2.S.x << " " << C2.S.y << " " << C2.r << endl;
  circle C3(point(1,7), point(8,6), point(7,-2));
  cout << C3.S.x << " " << C3.S.y << " " << C3.r << endl;
  circle K1(0, 0, 1);
  circle K2(0.2, 0, 0.8);
  P = intersect(K1, K2);
  cout << intersects(K1, K2) << endl;
  cout << P.first.x << " " << P.first.y << endl;
  cout << P.second.x << " " << P.second.y << endl;   
  pair<line,line> L = tangents(circle(2, 2, 1), point(0, 0));
  line T1 = L.first;
  line T2 = L.second;
  cout << T1.A << "x + " << T1.B << "y = " << T1.C << endl;
  cout << T2.A << "x + " << T2.B << "y = " << T2.C << endl;
  vector<point> x;
  x.push_back(point(3, 1));   
  x.push_back(point(1, 1));   
  x.push_back(point(2, 1));   
  x.push_back(point(4, 1));   
  x.push_back(point(2, 2));   
  x.push_back(point(3, 3));   
  x.push_back(point(5, 5));   
  x.push_back(point(2, 3));   
  x.push_back(point(3, 2));   
  x.push_back(point(1, 4));   
  x.push_back(point(1, 3));   
  x.push_back(point(1, 2));   
  cout << endl;
  convex_polygon A(x);
  for (int i = 0; i < A.size(); ++i) {
    cout << A[i].x << " " << A[i].y << endl;
  }
  return 0;
}
