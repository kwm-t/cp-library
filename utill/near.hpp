std::pair<long long, long long> near(long long x, long long m) {
	if (0 == x % m)return{ x,x + m };
	bool flg = false;
	if (x < 0) {
		flg = true;
		x *= -1;
	}
	long long mn = (x / m) * m;
	long long mx = mn + m;
	if (flg) {
		mn *= -1, mx *= -1;
		std::swap(mn, mx);
	}
	return { mn,mx };
}