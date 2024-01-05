long long ext_gcd(long long a, long long b, long long& x, long long &y) {
	//g = ax + by;
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