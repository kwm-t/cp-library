#include <vector>
namespace FastGCDConvolution {
	std::vector<bool> eratos(int N) {
		std::vector<bool> isprime(N, true);
		isprime[0] = isprime[1] = false;
		for (int p = 2; p < N; ++p) {
			if (!isprime[p]) continue;
			for (int i = p * 2; i < N; i += p)
				isprime[i] = false;
		}
		return isprime;
	}

	template<class T> void zeta(std::vector<T> &v, const std::vector<bool> &isprime) {
		int N = (int)v.size();
		for (int p = 2; p < N; ++p) {
			if (!isprime[p]) continue;
			for (int i = (N - 1) / p; i >= 1; --i)
				v[i] += v[i*p];
		}
	}

	template<class T> void mebius(std::vector<T> &v, const std::vector<bool> &isprime) {
		int N = (int)v.size();
		for (int p = 2; p < N; ++p) {
			if (!isprime[p]) continue;
			for (int i = 1; i*p < N; ++i)
				v[i] -= v[i*p];
		}
	}

	template<class T> std::vector<T> mul(const std::vector<T> &a, const std::vector<T> &b) {
		int N = max((int)a.size(), (int)b.size());
		const auto &isprime = eratos(N);
		std::vector<T> A(N, 0), B(N, 0);
		for (int i = 0; i < a.size(); ++i) A[i] = a[i];
		for (int i = 0; i < b.size(); ++i) B[i] = b[i];
		zeta(A, isprime), zeta(B, isprime);
		std::vector<T> C(N);
		for (int i = 1; i < N; ++i) C[i] = A[i] * B[i];
		mebius(C, isprime);
		return C;
	}
};