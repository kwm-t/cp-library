long long ext_gcd(long long a, long long b, long long& x, long long &y) {
	// gcd(a,b) = ax + by;
	if (0 == b) {
		x = 1;
		y = 0;
		return a;
	}
	long long q = a / b;
	long long g = ext_gcd(b, a - q * b, x, y);
	long long z = x - q * y;
	x = y;
	y = z;
	return g;
}
// ax + by = c (0 <= x and min. x)を満たすx,yを返す
// 存在しなければ{-1,-1}を返す
// ex. ax % b = cを満たすxの検索にも使える
std::pair<long long, long long> linearDiophantineEquation(long long a, long long b, long long c) {
	long long x, y;
	long long g = ext_gcd(a, b, x, y);
	if (c % g) return { -1,-1 };
	auto suba = a / g, subb = b / g, subc = c / g;
	x *= subc, y *= subc;
	x = (x % subb + subb) % subb;
	y = (c - a * x) / b;
	assert(a * x + b * y == c);
	return { x,y };
}