#include <vector>
//https://betrue12.hateblo.jp/entry/2019/05/25/105345
std::vector<int> morris_pratt(std::vector<int>& S) {
	int N = S.size();
	std::vector<int> A(N + 1), ret(N);
	A[0] = -1;
	int j = -1;
	for (int i = 0; i < N; i++) {
		while (j >= 0 && S[i] != S[j]) j = A[j];
		j++;
		A[i + 1] = ret[i] = j;
	}
	return ret;
}