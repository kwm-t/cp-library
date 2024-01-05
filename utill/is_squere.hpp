// •½•û”‚©‚Ç‚¤‚©‚Ì”»’è
pair<bool, long long> is_squere(long long N) {
	long long r = (long long)floor(sqrt((long double)N));  // Ø‚èÌ‚Ä‚µ‚½•½•ûª
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