#include <vector>
#include <functional>
template<class T>
T squareRootDecomposition(const int l, const int r, const int blockSize,
	const std::vector<T>&main, const std::vector<T>&sub,
	const std::function<T(T, T)> &op, const T &e) {
    // 遅いならベタ書き推奨
	T ret = e;
	int ll = ((l + blockSize - 1) / blockSize)*blockSize;
	int rr = (r / blockSize)*blockSize;
	for (int i = l; i < ll; ++i) {
		ret = op(ret, main[i]);
	}
	int l2 = ll / blockSize;
	int r2 = rr / blockSize;
	for (int i = l2; i < r2; i++) {
		ret = op(ret, sub[i]);
	}
	for (int i = rr; i < r; ++i) {
		ret = op(ret, main[i]);
	}
	return ret;
}