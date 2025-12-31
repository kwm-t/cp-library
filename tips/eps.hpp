const double EPS = 1e-10;
//等値比較
abs( a - b ) < EPS
//大小比較(どれでもよか)
a - EPS <= b
a <= b + EPS
a + EPS < b
a < b - EPS