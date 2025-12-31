//[l,r]
long long rangeAND(long long l, long long r) {
	long long ret = 0;
	rep(i, 64) {
		if (!(1 & (l >> i)))continue;
		if (!(1 & (r >> i)))continue;
		long long bit = 1ll << i;
		long long l0 = ((l / bit) / 2)*bit + l % bit;
		long long r0 = ((r / bit) / 2)*bit + r % bit;
		if ((r - l) == (r0 - l0))ret += 1LL << i;
	}
	return ret;
}