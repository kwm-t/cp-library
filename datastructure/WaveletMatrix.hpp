#include <cassert>
#include <vector>
#include <tuple>
int popcount(long long n) {
	n = (n & 0x5555555555555555) + (n >> 1 & 0x5555555555555555);
	n = (n & 0x3333333333333333) + (n >> 2 & 0x3333333333333333);
	n = (n & 0x0f0f0f0f0f0f0f0f) + (n >> 4 & 0x0f0f0f0f0f0f0f0f);
	n = (n & 0x00ff00ff00ff00ff) + (n >> 8 & 0x00ff00ff00ff00ff);
	n = (n & 0x0000ffff0000ffff) + (n >> 16 & 0x0000ffff0000ffff);
	n = (n & 0x00000000ffffffff) + (n >> 32 & 0x00000000ffffffff);
	return n;
}
// 完備辞書
struct SuccinctIndexableDictionary {
	int  length, blocks;
	std::vector<unsigned>bit, sum;
	// default constructor
	SuccinctIndexableDictionary() = default;
	// constructor(配列の長さを渡すブロックの大きさは2^5で固定)
	SuccinctIndexableDictionary(int _length) : length(_length), blocks((length + 31) >> 5) {
		bit.assign(blocks + 1, 0U);
		sum.assign(blocks + 1, 0U);
	}
	// k番目にbitをsetする
	void set(int k) {
		bit[k >> 5] |= 1U << (k & 31);
	}
	// setしてから呼び出す
	void build() {
		sum[0] = 0U;
		for (int i = 0; i < blocks - 1; ++i)sum[i + 1] = sum[i] + popcount(bit[i]);
	}
	// k番目のbitをgetする
	bool operator[](int k) {
		return (bool((bit[k >> 5] >> (k & 31)) & 1));
	}
	// [0,k)に立っているbitの数
	int rank(int k) {
		return (sum[k >> 5] + popcount(bit[k >> 5] & ((1U << (k & 31)) - 1)));
	}
	// [0,k)のvalの数
	int rank(bool val, int k) {
		return (val ? rank(k) : k - rank(k));
	}
	// select
	// rank(k+1)=n+1を満たす最小のl⇔rank(k)=nを満たす最大のk
	// つまりbitが立っている位置を記録した配列AのA[i]を取得している
	int select(int n) {
		if (rank(length) <= n)return -1;
		int ng = length + 1;
		int ok = 0;
		while (1 < abs(ng - ok)) {
			int c = (ok + ng) / 2;
			if (rank(c) <= n)ok = c;
			else ng = c;
		}
		return ok;
	}
};
template< typename T, int MAXLOG >
struct WaveletMatrix {
	int length;
	SuccinctIndexableDictionary matrix[MAXLOG];
	int mid[MAXLOG];
	// default constructor
	WaveletMatrix() = default;
	// constructor
	WaveletMatrix(std::vector< T > v) : length(v.size()) {
		std::vector< T > l(length), r(length);
		for (int i = MAXLOG - 1; i >= 0; --i) {
			matrix[i] = SuccinctIndexableDictionary(length + 1);
			int left = 0, right = 0;
			for (int j = 0; j < length; ++j) {
				if (((v[j] >> i) & 1)) {
					matrix[i].set(j);
					r[right++] = v[j];
				}
				else {
					l[left++] = v[j];
				}
			}
			// [0,left)[left,lef+right)
			mid[i] = left;
			matrix[i].build();
			for (int j = 0; j < right; ++j)l[j + left] = r[j];
			swap(v, l);
		}
	}
	// get:v[k]
	T access(int k) {
		T ret = 0;
		for (int i = MAXLOG - 1; i >= 0; --i) {
			bool flg = matrix[i][k];
			if (flg) ret |= T(1) << i;
			// １つ進んだ先のindex
			k = matrix[i].rank(flg, k) + mid[i] * flg;
		}
		return ret;
	}
	// get:v[k]
	T operator[](const int &k) {
		return access(k);
	}
	// [l,r)のbitの状態がfなもの区間遷移
	std::pair< int, int > succ(bool f, int l, int r, int level) {
		return { matrix[level].rank(f, l) + mid[level] * f, matrix[level].rank(f, r) + mid[level] * f };
	}
	// [0,r)に含まれるxの数を返す
	int rank(const T &x, int r) {
		int l = 0;
		for (int i = MAXLOG - 1; i >= 0; --i)tie(l, r) = succ((x >> i) & 1, l, r, i);
		return r - l;
	}
	// [l,r)に含まれるk番目に小さい(0-indexed)なものを返す
	T kth_smallest(int l, int r, int k) {
		assert(0 <= k && k < r - l);
		T ret = 0;
		for (int i = MAXLOG - 1; i >= 0; --i) {
			int cnt = matrix[i].rank(false, r) - matrix[i].rank(false, l);
			bool flg = cnt <= k;
			if (flg) {
				ret |= T(1) << i;
				k -= cnt;
			}
			std::tie(l, r) = succ(flg, l, r, i);
		}
		return ret;
	}
	// [l,r)に含まれるk番目に大きい(0-indexed)なものを返す
	T kth_largest(int l, int r, int k) {
		return kth_smallest(l, r, r - l - k - 1);
	}
	// [l,r)に含まれるv[i] < upperな要素の数
	int range_freq(int l, int r, T upper) {
		int ret = 0;
		for (int i = MAXLOG - 1; i >= 0; --i) {
			bool flg = ((upper >> i) & 1);
			if (flg) ret += matrix[i].rank(false, r) - matrix[i].rank(false, l);
			std::tie(l, r) = succ(flg, l, r, i);
		}
		return ret;
	}
	// [l,r)に含まれるlower <= v[i] < upperな要素の数
	int range_freq(int l, int r, T lower, T upper) {
		return range_freq(l, r, upper) - range_freq(l, r, lower);
	}
	// [l,r)に含まれるupperの次に大きいもの
	T prev_value(int l, int r, T upper) {
		int cnt = range_freq(l, r, upper);
		return cnt == 0 ? T(-1) : kth_smallest(l, r, cnt - 1);
	}
	// [l,r)に含まれるlower以上のもの
	T next_value(int l, int r, T lower) {
		int cnt = range_freq(l, r, lower);
		return cnt == r - l ? T(-1) : kth_smallest(l, r, cnt);
	}
};
template< typename T, int MAXLOG >
struct CompressedWaveletMatrix {
	WaveletMatrix< int, MAXLOG > mat;
	vector< T > ys;
	CompressedWaveletMatrix(const vector< T > &v) : ys(v) {
		sort(begin(ys), end(ys));
		ys.erase(unique(begin(ys), end(ys)), end(ys));
		vector< int > t(v.size());
		for (int i = 0; i < v.size(); i++) t[i] = get(v[i]);
		mat = WaveletMatrix< int, MAXLOG >(t);
	}
	inline int get(const T& x) {
		return lower_bound(begin(ys), end(ys), x) - begin(ys);
	}
	T access(int k) {
		return ys[mat.access(k)];
	}
	T operator[](const int &k) {
		return access(k);
	}
	int rank(const T &x, int r) {
		auto pos = get(x);
		if (pos == ys.size() || ys[pos] != x) return 0;
		return mat.rank(pos, r);
	}
	T kth_smallest(int l, int r, int k) {
		return ys[mat.kth_smallest(l, r, k)];
	}
	T kth_largest(int l, int r, int k) {
		return ys[mat.kth_largest(l, r, k)];
	}
	int range_freq(int l, int r, T upper) {
		return mat.range_freq(l, r, get(upper));
	}
	int range_freq(int l, int r, T lower, T upper) {
		return mat.range_freq(l, r, get(lower), get(upper));
	}
	T prev_value(int l, int r, T upper) {
		auto ret = mat.prev_value(l, r, get(upper));
		return ret == -1 ? T(-1) : ys[ret];
	}
	T next_value(int l, int r, T lower) {
		auto ret = mat.next_value(l, r, get(lower));
		return ret == -1 ? T(-1) : ys[ret];
	}
};
// Rectangle Sum
// https://ei1333.github.io/library/structure/wavelet/wavelet-matrix-rectangle-sum.hpp
// https://atcoder.jp/contests/abc405/submissions/65714285