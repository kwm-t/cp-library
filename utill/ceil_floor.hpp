auto ceil_div = [](long long n, long long d) {
	return n / d + ((n ^ d) > 0 && n % d != 0);
};
auto floor_div = [](long long n, long long d) {
	return n / d - ((n ^ d) < 0 && n % d != 0);
};
inline long long ceil_div(long long x, long long y) { return x / y + ((x ^ y) > 0 && x % y != 0); }
inline long long floor_div(long long x, long long y) { return x / y - ((x ^ y) < 0 && x % y != 0); }