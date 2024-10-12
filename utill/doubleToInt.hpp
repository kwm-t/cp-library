#include <string>
long long DoubleChangeInt(std::string s, int n) {
	//小数点以下n桁で与えられるS(double)を
	//10^n倍したintで返す
	long long ret = 0;
	int dotPos = s.find(".");
	int plusZero;
	if (-1 == dotPos) {
		s += ".";
		plusZero = n;
	}
	else plusZero = n - (s.size() - dotPos - 1);
	for (int i = 0; i < plusZero; ++i) s += "0";
	for (int i = 0; i < (int)s.size(); ++i) {
		if ('.' == s[i]) continue;
		ret = ret * 10 + (s[i] - '0');
	}
	return ret;
}
long long NearNum(long long n, long long d, bool b) {
	/*
	true 532 -> 600, -532 -> -500
	false 532-> 500, -532 -> -600
	*/
	long long ret = n;
	bool bminus = false;
	if (n < 0) {
		bminus = true;
		ret = -ret;
	}
	if (b != bminus) ret = ((ret + d - 1) / d)*d;
	else ret = (ret / d)*d;
	if (bminus) ret = -ret;
	return ret;
}