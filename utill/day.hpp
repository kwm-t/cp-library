#include <vector>
#include <string>
// 0001/01/01からの経過日数
int days(int y, int m, int d) {
	std::vector<int>day = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	//(1,1,1)->1, (1,1,2)->2, ...
	int ret = (y - 1) * 365 + (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400;
	for (int i = 1; i < m; ++i) ret += day[i - 1];
	ret += d;
	if ((m > 2) && (((0 == y % 4) && (0 != y % 100)) || (0 == y % 400)))ret++;
	return ret;
}
// YYYY-MM-DD からy,m,dを抜き出す関数
void f(std::string s, int & s0, int&s1, int &s2) {
	s0 = 0, s1 = 0, s2 = 0;
	for (int i = 0; i < 4; ++i)s0 = s0 * 10 + s[i] - '0';
	for (int i = 5; i < 7; ++i) s1 = s1 * 10 + s[i] - '0';
	for (int i = 8; i < 10; ++i) s2 = s2 * 10 + s[i] - '0';
}