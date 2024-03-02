#include <string>
#include <vector>
#include <atcoder/all>
bool contain(const std::string &s, const std::string &t) {
	// •¶Žš—ñs‚ª•¶Žš—ñt‚ÉŠÜ‚Ü‚ê‚Ä‚¢‚é‚©‚Ì”»’è
	int n = s.size(), m = t.size();
	std::vector<int> z = atcoder::z_algorithm(s + t);
	for (int i = n; i <= m; ++i) {
		if (z[i] >= n)return true;
	}
	return false;
}