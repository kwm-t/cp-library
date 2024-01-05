#include <utility>
#include "sqrt.hpp"
const long long LINF = 1e18;

//êÆêîâÇ™ë∂ç›ÇµÇ»ÇØÇÍÇŒ{-LINF,LINF}
std::pair<long long, long long> calc(long long a, long long b, long long c) {
	long long d = _sqrt(b * b - 4LL * a * c);
	if (-1 == d)return { -LINF,LINF };
	long long l = -1LL * b - d;
	long long r = -1LL * b + d;
	if (0 != l % (2LL * a)) l = -LINF;
	else l /= (2LL * a);
	if (0 != r % (2LL * a)) r = LINF;
	else r /= (2LL * a);
	return { l,r };
}