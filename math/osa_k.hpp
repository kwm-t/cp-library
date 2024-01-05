#include <vector>
//自分の持つ最小の素因数を持つテーブルを作成する
//a /= osk[a]と a == 1になるまで続けると素因数分解に相当する
// osa_k法
int osa_k[1000006];
void sieve(int max = 1000006) {
	osa_k[1] = 1;
	for (int i = 2; i < max; ++i) {
		if (0 != osa_k[i])continue;
		osa_k[i] = i;
		for (int j = i * 2; j <= max; j += i) {
			if (0 == osa_k[j]) osa_k[j] = i;
		}
	}
}
std::vector<int> calc(int n) {
	std::vector<int>ret;
	while (1 != n) {
		ret.push_back(osa_k[n]);
		n /= osa_k[n];
	}
	return ret;
}