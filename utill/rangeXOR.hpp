// [0,r)‚ÌXOR
long long rangeXOR(long long r) {
	r--;
	if (r <= 0)return 0;
	long long ret = 0;
	while (true) {
		ret ^= r;
		r--;
		if (0 == r || 3 == r % 4)break;
	}
	return ret;
}
// [l,r)‚Ìxor
long long rangeXOR(long long l, long long r) {
	return rangeXOR(r) ^ rangeXOR(l);
}
// 01234
// 12345
// 23456‚Ì‚æ‚¤‚ÈƒOƒŠƒbƒh‚Ìxor
long long gridXOR(long long n, long long m) {
	long long ret = 0;
	if (1 == n % 2) {
		ret ^= rangeXOR(n - 1, n + m - 1);
		n--;
	}
	if (0 == n)return ret;
	if (1 == m % 2) {
		ret ^= rangeXOR(m - 1, n + m - 1);
		m--;
	}
	if (0 == m)return ret;
	ret ^= 2 * (rangeXOR(0, n / 2));
	ret ^= 2 * (rangeXOR(m / 2, (n + m) / 2));
	return ret;
}