#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <iomanip>
#include <vector>
#include "MiniParser.hpp"

using namespace std;

double mpxy(MiniParser& mp, double x, double y) {
  MiniParser::IDMap ids;
  ids["x"] = x;
  ids["y"] = y;
  return mp.eval(ids);
}

double rk(double yn, double h, double k1, double k2, double k3, double k4) {
  return yn + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}

double rkk1(MiniParser& mp, double xn, double yn) {
  return mpxy(mp, xn, yn);
}

double rkk2(MiniParser& mp, double xn, double yn, double h, double k1) {
  double x = xn + h / 2, y = yn + h * k1 / 2;
  return mpxy(mp, x, y);
}

double rkk3(MiniParser& mp, double xn, double yn, double h, double k2) {
  double x = xn + h / 2, y = yn + h * k2 / 2;
  return mpxy(mp, x, y);
}

double rkk4(MiniParser& mp, double xn, double yn, double h, double k3) {
  double x = xn + h, y = yn + h * k3;
  return mpxy(mp, x, y);
}

int main (int argc, char *argv[]) {
  ifstream cin("input.txt");
  //ofstream cout("output.txt");
  string fun;
  getline(cin, fun);
  double a, b, x0, y0;
  int n;
  cin >> a >> b >> x0 >> y0 >> n;
  double h = (b - a) / n;
  MiniParser mp(fun);
  vector< vector<double> > e(n+1, vector<double>(4, 0));
  e[0][0] = x0; e[0][1] = y0;
  e[0][2] = mpxy(mp, e[0][0], e[0][1]);
  e[0][3] = h * e[0][2];
  for (int i = 1; i <= n; i++) {
    e[i][0] = h * i + x0;
    e[i][1] = e[i - 1][1] + e[i - 1][3];
    e[i][2] = mpxy(mp, e[i][0], e[i][1]);
    e[i][3] = h * e[i][2];
  }
  cout << "f(x, y) = " << fun << '\n';
  cout << "y(" << x0 << ")=" << y0 << ' ';
  cout << "[" << a << "," << b << "] ";
  cout << "f(" << x0 << ", " << y0 << ")=" << mpxy(mp, x0, y0) << "\n";
  cout << "Эйлера:\n";
  cout << setw(6) << "i" << setw(11) << "xi" << setw(11) << "yi"
       << setw(11) << "f(xi, yi)" << setw(16) << "h * f(xi, yi)" << "\n";
  for (int i = 0; i <= n; i++) {
    cout << setw(6) << i << " " << setw(10) << e[i][0] << " "
         << setw(10) << e[i][1] << " "
         << setw(10) << e[i][2] << " "
         << setw(15) << e[i][3] << "\n";
  }
  e.clear();
  e.resize(n+1, vector<double>(7, 0));
  e[0][0] = x0; e[0][1] = y0;
  e[0][2] = e[0][0] + h / 2;
  e[0][3] = mpxy(mp, e[0][0], e[0][1]);
  e[0][4] = e[0][1] + h * e[0][3] / 2;
  e[0][5] = mpxy(mp, e[0][2], e[0][4]);
  e[0][6] = h * e[0][5];
  for (int i = 1; i <= n; i++) {
    e[i][0] = h * i + x0;
    e[i][1] = e[i - 1][1] + e[i - 1][6];
    e[i][2] = e[i][0] + h / 2;
    e[i][3] = mpxy(mp, e[i][0], e[i][1]);
    e[i][4] = e[i][1] + h * e[i][3] / 2;
    e[i][5] = mpxy(mp, e[i][2], e[i][4]);
    e[i][6] = h * e[i][5];
  }
  cout << "Эйлера +:\n";
  cout << setw(6) << "i" << setw(11) << "xi"
       << setw(11) << "yi"
       << setw(11) << "xi+h/2"
       << setw(11) << "f(xi, yi)"
       << setw(11) << "yi+h/2*f"
       << setw(21) << "f(xi+h/2, yi+h/2*f)"
       << setw(11) << "dyi"
       << "\n";
  for (int i = 0; i <= n; i++) {
    cout << setw(6) << i << " "
         << setw(10) << e[i][0] << " "
         << setw(10) << e[i][1] << " "
         << setw(10) << e[i][2] << " "
         << setw(10) << e[i][3] << " "
         << setw(10) << e[i][4] << " "
         << setw(20) << e[i][5] << " "
         << setw(10) << e[i][6] << "\n";
  }
  vector< vector<double> > m(n+1, vector<double>(7, 0));
  m[0][0] = x0; m[0][1] = y0;
  m[0][2] = rkk1(mp, m[0][0], m[0][1]);
  m[0][3] = rkk2(mp, m[0][0], m[0][1], h, m[0][2]);
  m[0][4] = rkk3(mp, m[0][0], m[0][1], h, m[0][3]);
  m[0][5] = rkk4(mp, m[0][0], m[0][1], h, m[0][4]);
  m[0][6] = mpxy(mp, m[0][0], m[0][1]);
  for (int i = 1; i <= 2; i++) {
    m[i][0] = h * i + x0;
    m[i][1] = rk(m[i - 1][1], h, m[i - 1][2], m[i - 1][3], m[i - 1][4], m[i - 1][5]);
    m[i][2] = rkk1(mp, m[i][0], m[i][1]);
    m[i][3] = rkk2(mp, m[i][0], m[i][1], h, m[i][2]);
    m[i][4] = rkk3(mp, m[i][0], m[i][1], h, m[i][3]);
    m[i][5] = rkk4(mp, m[i][0], m[i][1], h, m[i][4]);
    m[i][6] = mpxy(mp, m[i][0], m[i][1]);
  }
  for (int i = 3; i <= n; i++) {
    m[i][0] = h * i + x0;
    m[i][1] = m[i - 1][1] + h * (23 * m[i - 1][6] - 16 * m[i - 2][6] + 5 * m[i - 3][6]) / 12;
    m[i][6] = mpxy(mp, m[i][0], m[i][1]);
  }
  cout << "Адамса:\n";
  cout << setw(6) << "i" << setw(11) << "xi" << setw(11) << "yi"
       << setw(11) << "k1" << setw(11) << "k2"
       << setw(11) << "k3" << setw(11) << "k4"
       << setw(11) << "f(xi, yi)" << "\n";
  for (int i = 0; i <= n; i++) {
    cout << setw(6) << i << " " << setw(10) << m[i][0] << " "
         << setw(10) << m[i][1] << " "
         << setw(10) << m[i][2] << " "
         << setw(10) << m[i][3] << " "
         << setw(10) << m[i][4] << " "
         << setw(10) << m[i][5] << " "
         << setw(10) << m[i][6] << "\n";
  }
  cout << "Compare:\n";
  cout << setw(6) << "i" << setw(11) << "E" << setw(11) << "A" << "\n";
  for (int i = 0; i <= n; i++) {
    cout << setw(6) << i << " " << setw(10) << e[i][1] << " "
         << setw(10) << m[i][1] << " ""\n";
  }
}
