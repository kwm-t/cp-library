#include <vector>
#include <string>
std::vector<std::pair<char, int>> RunLengthEncoding(std::string& s) {
	std::vector<std::pair<char, int>> ret;
	for (int i = 0; i < s.size();) {
		int c = 1;
		int j = i + 1;
		while (j != s.size() && s[j] == s[i]) j++;
		ret.push_back({ s[i],j - i });
		i = j;
	}
	return ret;
}