#include <string>
#include <algorithm>

long long exchange2to10(std::string s) {
	long long ret = 0;
	std::reverse(s.begin(), s.end());
	for (int i = 0; i < std::min(60, (int)s.size()); ++i) {
		if ('1' == s[i])ret += (1LL) << i;
	}
	return ret;
}