#include <vector>
template<typename T>
std::vector<vector<T>> matrixMul(const std::vector<std::vector<T>>&A, const std::vector<std::vector<T>>&B) {
	std::vector<vector<T>> C(A.size(), std::vector<T>(B[0].size()));
	for (int i = 0; i < (int)A.size(); ++i) {
		for (int k = 0; k < (int)A[0].size(); ++k) {
			for (int j = 0; j < (int)B[0].size(); ++j) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return C;
}
template<typename T>
std::vector<std::vector<T>> matrixPow(long long n, std::vector<std::vector<T>> mat) {
	int size = (int)mat.size();
	vector<vector<mint>> mret(size, vector<mint>(size));
	for (int i = 0; i < size; ++i) {
		mret[i][i] = 1;
	}
	while (n > 0) {
		if (1 & n) mret = matrixMul(mat, mret);
		mat = matrixMul(mat, mat);
		n >>= 1;
	}
	return mret;
}