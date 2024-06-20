// 平方数かどうかの判定
pair<bool, long long> is_squere(long long N) {
	long long r = (long long)floor(sqrt((long double)N));  // 切り捨てした平方根
	return { (r * r) == N ,r };
}
bool squareCheck(long long n) {
	long long ng = 0;
	long long ok = INF;
	while (1 < abs(ok - ng)) {
		long long c = (ng + ok) / 2;
		if (n <= (c * c)) ok = c;
		else ng = c;
	}
	bool ret = false;
	if (n == ok * ok)ret = true;
	return ret;
}