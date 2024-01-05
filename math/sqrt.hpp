#include <cmath>
const int INF = 1e9;

// ë∂ç›ÇµÇ»ÇØÇÍÇŒ -1 Ç return
long long _sqrt(long long x) {
	if (x < 0)return -1;
	long long ng = -1;
	long long ok =  3 * (long long)INF + 1;
	while (1 < abs(ok - ng)) {
		long long c = (ng + ok) / 2;
		auto check = [&]() {
			return x <= c * c;
		};
		if (check()) ok = c;
		else ng = c;
	}
	long long ret = ok;
	if (ret * ret != x)ret = -1;
	return ret;
}