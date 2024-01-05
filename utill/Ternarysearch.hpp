#include <iostream> 
#include <algorithm>
const long long LINF = 1e18;
double Ternarysearch() {
	long long a, b; std::cin >> a >> b;
	long long r = LINF;
	long long l = 0;
	auto f = [&](long long x)->double {
		return (double)a / sqrt(1.0 + x) + (double)b * x;
	};
	// ‰º‚É“Ê
	while (2 < abs(r - l)) {
		long long ml = (l * 2 + r) / 3;
		long long mr = (l + r * 2) / 3;
		if (f(ml) < f(mr)) r = mr;
		else l = ml;
	}
	double ans = a;
	for (long long i = l; i <= r; ++i)ans = std::min(ans, f(i));
	return ans;
	printf("%.10Lf\n", ans);
}