#include <string>
#include <algorithm>

std::string exchange10to2(long long num) {
	if (0 == num)return "0";
	std::string ret;
	while (num) {
		if (1 == num % 2)ret += '1';
		else ret += '0';
		num /= 2;
	}
	std::reverse(ret.begin(), ret.end());
	return ret;
}