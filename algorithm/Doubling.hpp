#ifndef DOUBLING_HPP
#define DOUBLING_HPP

#include <vector>
#include <functional>
#include <utility>

/**
 * @brief Doubling (Binary Lifting)
 *
 * 関数 f の k 回適用 f^k(x) を高速に求めるデータ構造。
 * nxt[v] = f(v) としたとき、
 * 2^i 回遷移した先を前計算することで O(log k) で遷移可能。
 *
 * value を持たせることで、遷移に伴う値（距離・重み・xor など）の
 * 累積も同時に計算できる。
 *
 * 典型用途:
 *   ・Functional Graph の k step
 *   ・テレポート問題
 *   ・祖先ジャンプ
 *   ・遷移コストの累積
 *
 * 計算量:
 *   構築 O(N log K)
 *   jump / jump_value / first_true : O(log K)
 *
 * @tparam T      遷移に付随する値の型
 * @tparam Merge  値をマージする関数 (例: std::plus)
 *
 * @param nxt     nxt[v] = v から1回遷移した先
 * @param value   各遷移に対応する値
 * @param log     最大ステップ数のビット長
 *
 * 制約 / 注意:
 *   ・first_true は単調性が必要
 *       f(jump(v,k)) が
 *       false false ... false true true ...
 *       の形になる必要がある
 *
 * 使用例:
 *   vector<int> nxt = {...};
 *   Doubling<> db(nxt);
 *   int v = db.jump(0, 1e18);
 *
 *   vector<long long> cost = {...};
 *   auto merge = [](auto a, auto b) { return a + b; };
 *   Doubling<long long, decltype(merge)> db2(nxt, cost, merge);
 *   auto [to, sum] = db2.jump_value(0, k);
 * 
 * verified:
 *   https://atcoder.jp/contests/awc0025/submissions/74048140
 *   https://atcoder.jp/contests/abc438/submissions/74048565
 */

template<class T = int, class Merge = std::plus<T>>
class Doubling {
public:
	int N;
	int LOG;

	Doubling(const std::vector<int>& nxt,
		int log = 60)
		: N((int)nxt.size()), LOG(log)
	{
		init(nxt);
	}

	Doubling(const std::vector<int>& nxt,
		const std::vector<T>& value,
		Merge m = Merge(),
		int log = 60)
		: N((int)nxt.size()), LOG(log), merge(m)
	{
		init(nxt, value);
	}

	// k回遷移
	int jump(int v, long long k) const {
		for (int i = 0; i < LOG; ++i) {
			if (k >> i & 1) v = to[i][v];
		}
		return v;
	}

	// value付き k回遷移
	std::pair<int, T> jump_value(int v, long long k) const {
		T res{};
		for (int i = 0; i < LOG; ++i) {
			if (k >> i & 1) {
				res = merge(res, val[i][v]);
				v = to[i][v];
			}
		}
		return { v, res };
	}

	// f(jump(v,k)) が true になる最小 k (存在しなければ -1)
	template<class F>
	long long first_true(int v, long long limit, F f) const {
		if (f(v)) return 0;

		long long k = 0;

		for (int i = LOG - 1; i >= 0; --i) {
			if (k + (1LL << i) > limit) continue;

			int nv = to[i][v];

			if (!f(nv)) {
				v = nv;
				k += 1LL << i;
			}
		}

		if (k == limit) return -1;
		return k + 1;
	}

private:

	std::vector<std::vector<int>> to;
	std::vector<std::vector<T>> val;
	Merge merge;

	void init(const std::vector<int>& nxt) {
		to.assign(LOG, std::vector<int>(N));
		to[0] = nxt;

		for (int i = 1; i < LOG; ++i) {
			for (int v = 0; v < N; ++v) {
				to[i][v] = to[i - 1][to[i - 1][v]];
			}
		}
	}

	void init(const std::vector<int>& nxt, const std::vector<T>& value) {
		to.assign(LOG, std::vector<int>(N));
		val.assign(LOG, std::vector<T>(N));

		to[0] = nxt;
		val[0] = value;

		for (int i = 1; i < LOG; ++i) {
			for (int v = 0; v < N; ++v) {
				int u = to[i - 1][v];
				to[i][v] = to[i - 1][u];
				val[i][v] = merge(val[i - 1][v], val[i - 1][u]);
			}
		}
	}
};

#endif // DOUBLING_HPP