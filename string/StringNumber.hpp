std::string add(const std::string &s, int d) {
	int n = d;
	std::string ret;
	for (int i = (int)s.size() - 1; i >= 0; --i) {
		n += s[i] - '0';
		ret += (n % 10) + '0';
		n /= 10;
	}
	if (0 != n)ret += n + '0';
	std::reverse(ret.begin(), ret.end());
	return ret;
}
std::string divide(const std::string &s, int d) {
	int n = 0;
	std::string ret;
	for (int i = 0; i < (int)s.size(); ++i) {
		n = n * 10 + s[i] - '0';
		int x = n / d;
		n %= d;
		if (0 == x && 0 == i)continue;
		ret += x + '0';
	}
	return ret;
}
long long mod(const std::string &s, long long m) {
	long long ret = 0;
	for (int i = 0; i < (int)s.size(); ++i) {
		ret = ret * 10 + s[i] - '0';
		ret %= m;
	}
	return ret;
}