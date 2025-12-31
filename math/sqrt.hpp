#include <cmath>

long long floorSqrt(long long x) {
	const long long INF = 1e9;
	if (x < 0)return -1;
	long long ok = 0;
	long long ng = 3 * (long long)INF + 1;
	while (1 < abs(ok - ng)) {
		long long c = (ng + ok) / 2;
		auto check = [&]() {
			return c * c <= x;
		};
		if (check()) ok = c;
		else ng = c;
	}
	return ok;
}