#include <vector>
// 添字xor畳み込み(アダマール変換)
template <typename T>
void walsh_hadamard_transform(std::vector<T>& f) {
	int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				// 添字 xor
				T x = f[j], y = f[j | i];
				f[j] = x + y, f[j | i] = x - y;
				/*
					// 添字 or
					 f[j | i] += f[j];
				*/
				/*
					// 添字 and
					f[j] += f[j | i];
				*/
			}
		}
	}
}
template <typename T>
void walsh_hadamard_transform_inv(std::vector<T>& f) {
	int n = f.size();
	T inv2 = T(2).inv();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				// 添字 xor
				T x = f[j], y = f[j | i];
				f[j] = (x + y) * inv2, f[j | i] = (x - y) * inv2;
				/*
					// 添字 or
					f[j | i] -= f[j];
				*/
				/*
					// 添字 and
					f[j] -= f[j | i];
				*/
			}
		}
	}
}
template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
	std::vector<T> c(a.size());
	for (int i = 0;i < a.size();++i) c[i] = a[i] + b[i];
	return c;
}
template <typename T>
std::vector<T> operator*(std::vector<T> a, std::vector<T> b) {
	walsh_hadamard_transform(a);
	walsh_hadamard_transform(b);
	for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
	walsh_hadamard_transform_inv(a);
	return a;
}