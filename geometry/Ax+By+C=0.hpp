tuple<long long, long long, long long>segment(P p1, P p2) {
	long long a = p2.second - p1.second;
	long long b = p1.first - p2.first;
	long long c = p1.second * p2.first - p1.first * p2.second;
	long long g = gcd(a, gcd(b, c));
	a /= g, b /= g, c /= g;
	if (a < 0) a *= -1, b *= -1, c *= -1;
	else if (0 == a) {
		if (b < 0) b *= -1, c *= -1;
		else if (0 == b) c = abs(c);
	}
	return { a,b,c };
}