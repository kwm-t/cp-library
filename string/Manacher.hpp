#include <string>
#include <vector>
#include <algorithm>
// Sの各文字に対して、その文字を中心とする最大サイズの回分の半径のサイズを返す
// 偶数長の回文を検知するためには特別処理が必要
std::vector<int> manacher(const std::string& s) {
	std::vector< int > radius(s.size());
	int i = 0, j = 0;
	while (i < s.size()) {
		while (i - j >= 0 && i + j < s.size() && s[i - j] == s[i + j]) {
			++j;
		}
		radius[i] = j;
		int k = 1;
		while (i - k >= 0 && i + k < s.size() && k + radius[i - k] < j) {
			radius[i + k] = radius[i - k];
			++k;
		}
		i += k;
		j -= k;
	}
	return radius;
}
// Sの各文字に対して、その文字から始まる最大の長さの回文サイズを返す
std::vector<int>manacherSub(std::string s) {
	std::string str;
	for (int i = 0; i < (int)s.size(); ++i) {
		if (0 != str.size())str += '*';
		str += s[i];
	}
	auto v = manacher(str);
	std::vector<int>res(s.size());
	for (int i = 0; i < (int)v.size(); ++i) {
		int sz, l;
		if (0 == i % 2) {
			sz = (v[i] + 1) / 2;
			l = i / 2 + 1 - sz;
			sz = sz * 2 - 1;
		}
		else {
			sz = v[i] / 2;
			l = (i + 1) / 2 - sz;
			sz = sz * 2;
		}
		res[l] = std::max(res[l], sz);
	}
	for(int i = 1;i < res.size();++i) res[i] = std::max(res[i], res[i - 1] - 2);
	return res;
}

template<typename T>
std::vector<int> manacher(const std::vector<T>& s) {
	std::vector<int> radius(s.size());
	int i = 0, j = 0;
	while (i < s.size()) {
		while (i - j >= 0 && i + j < s.size() && s[i - j] == s[i + j]) {
			++j;
		}
		radius[i] = j;
		int k = 1;
		while (i - k >= 0 && i + k < s.size() && k + radius[i - k] < j) {
			radius[i + k] = radius[i - k];
			++k;
		}
		i += k;
		j -= k;
	}
	return radius;
}
template<typename T>
std::vector<int> manacherWrapped(const std::vector<T>& v, const T dummy) {
	if((int)v.size() == 0)return {};
	std::vector<T>w((int)v.size() * 2 - 1, dummy);
	for (int i = 0; i < (int)v.size(); ++i)w[i * 2] = v[i];
	auto mn = manacher(w);
	std::vector<int>result(mn.size());
	for (int i = 0; i < (int)mn.size(); ++i) {
		if (i % 2 == 0) result[i] = (mn[i] + 1) / 2;
		else result[i] += (mn[i] + 0) / 2;
	}
	return result;
}