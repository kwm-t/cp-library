// LINFを超える場合は-1を返す
long long safePow(long long p, int  q, const long long LINF = 1e18) {
	long long val = 1;
	while (q > 0) {
		if (1 & q) {
			if (val > LINF / p) return -1;
			val *= p;
		}
		if (p > LINF / p) p = LINF + 1;
		else p *= p;
		q >>= 1;
	}
	return val;
}