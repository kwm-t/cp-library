class Point {
public:
	int x, y;

	Point(int x = 0, int y = 0) : x(x), y(y) {}

	long long cross(const Point& other) const {
		return 1LL * x * other.y - 1LL * y * other.x;
	}
	// θでsort
	bool operator<(const Point& o) const {
		int h1 = (y < 0 || (y == 0 && x < 0));
		int h2 = (o.y < 0 || (o.y == 0 && o.x < 0));
		if (h1 != h2) return h1 < h2;

		long long cr = cross(o);
		if (cr != 0) return cr > 0;

		// 同一直線上の場合は原点からの距離が近い方
		return 1LL * x * x + 1LL * y * y < 1LL * o.x * o.x + 1LL * o.y * o.y;
	}
};
// 同じ角度のが複数ある場合は、角度のみが必要な時は
// {x,y}->{x/gcd(x,y),y/gcd(x,y)} とかして適当に
// https://atcoder.jp/contests/abc442/submissions/72734656
