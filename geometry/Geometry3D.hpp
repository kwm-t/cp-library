#include <array>
#include <numeric>

using point = std::array<long long, 3>;
using line = std::array<point, 2>;
class frac {
public:
	long long nume, deno;
	frac() : nume(0), deno(1) {}
	frac(long long n) : nume(n), deno(1) {}
	frac(long long n, long long d) {
		if (d < 0) {
			nume = -n, deno = -d;
		}
		else {
			nume = n, deno = d;
		}
	}
	void reduce() {//約分
		long long g = std::gcd(nume, deno);
		nume /= g;
		deno /= g;
	}
	frac operator-(long long n) { return frac(nume - n * deno, deno); }
	frac operator+(long long n) { return frac(nume + n * deno, deno); }
	frac operator*(frac&x) { return frac(nume*x.nume, deno*x.deno); }
	bool operator<(long long x) { return nume < x*deno; }
	friend bool operator==(const frac&x, const frac&y) { return x.nume*y.deno == x.deno*y.nume; }
	friend bool operator<(const frac&x, const frac&y) { return x.nume*y.deno < y.nume*x.deno; }
};
std::array<frac, 2> get_vec(point&p, point&q) {
	frac dy = frac(p[1] - q[1], p[0] - q[0]);
	frac dz = frac(p[2] - q[2], p[0] - q[0]);
	return { dy,dz };
}
using point_frac = std::array<frac, 3>;
std::pair<point_frac, bool> get_intersect(line l1, line l2) {
	// z=0に射影する交点(x,y)を求める。
	//条件::平行でない、x=aの平面上にない
	long long deno = (l1[0][0] - l1[1][0])*(l2[0][1] - l2[1][1]) - (l2[0][0] - l2[1][0])*(l1[0][1] - l1[1][1]);
	if (!deno) {
		// 平行でないのが射影の結果並行扱いになったので他の面に射影するためにswapする
		std::swap(l1[0][1], l1[0][2]);
		std::swap(l1[1][1], l1[1][2]);
		std::swap(l2[0][1], l2[0][2]);
		std::swap(l2[1][1], l2[1][2]);
		auto[p, f] = get_intersect(l1, l2);
		std::swap(p[1], p[2]);
		return{ p,f };
	}
	// メイン処理
	long long xnume = l1[0][0] * l2[0][0] * (l1[1][1] - l2[1][1]) - l1[0][0] * l2[1][0] * (l1[1][1] - l2[0][1]) - l1[1][0] * l2[0][0] * (l1[0][1] - l2[1][1]) + l1[1][0] * l2[1][0] * (l1[0][1] - l2[0][1]);
	long long ynume = -(l1[0][1] * l2[0][1] * (l1[1][0] - l2[1][0]) - l1[0][1] * l2[1][1] * (l1[1][0] - l2[0][0]) - l1[1][1] * l2[0][1] * (l1[0][0] - l2[1][0]) + l1[1][1] * l2[1][1] * (l1[0][0] - l2[0][0]));
	frac x = frac(xnume, deno), y = frac(ynume, deno);
	// z座標が一致すれば交点を持つ
	frac dz1 = frac(l1[0][2] - l1[1][2], l1[0][0] - l1[1][0]);
	frac dz2 = frac(l2[0][2] - l2[1][2], l2[0][0] - l2[1][0]);
	frac z1 = (x - l1[0][0])*dz1 + l1[0][2];
	frac z2 = (x - l2[0][0])*dz2 + l2[0][2];
	if (z1 == z2)return { {x,y,z1},true };
	return { {},false };
}