// [l,r]‚Ìxor
long long rangeXOR(long long l, long long r) {
	long long ret = 0;
	while ((l <= r) && (0 != l % 4)) {
		ret ^= l;
		l++;
	}
	while ((l <= r) && (3 != r % 4)) {
		ret ^= r;
		r--;
	}
	return ret;
}