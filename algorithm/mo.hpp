#ifndef KWM_T_ALGORITHM_MO_HPP
#define KWM_T_ALGORITHM_MO_HPP

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

/**
 * @brief Mo's Algorithm（区間クエリ順序最適化）
 *
 * クエリ順を並び替えて、区間の追加・削除を amortized に抑える
 *
 * 典型用途:
 *   - 区間内 distinct 数
 *   - 転倒数
 *   - 頻度管理系
 *
 * 計算量:
 *   - O((N + Q) * sqrt(N))
 *
 * @tparam AddL   左に追加 (l--)
 * @tparam AddR   右に追加 (r++)
 * @tparam RemL   左を削除 (l++)
 * @tparam RemR   右を削除 (r--)
 * @tparam Out    クエリ結果出力
 *
 * 制約 / 注意:
 *   - クエリは [l, r) の半開区間
 *   - add/remove は順序に依存しないように設計すること
 *
 * verified:
 *   https://atcoder.jp/contests/awc0015/submissions/74250351
 *	 https://yukicoder.me/submissions/1153333
 *   半開区間でない一般の場合
 * 　https://atcoder.jp/contests/abc463/submissions/76859533
 */
namespace kwm_t::algorithm {

class Mo {
private:
	int n, q;
	int block_size;

	std::vector<int> left, right;
	std::vector<int> order;

public:
	Mo(int n_, int q_) : n(n_), q(q_) {
		block_size = std::max(1, int(n / std::sqrt(q_)));
		order.resize(q_);
		std::iota(order.begin(), order.end(), 0);
	}

	// クエリ設定
	void query(const std::vector<int>& l, const std::vector<int>& r) {
		left = l;
		right = r;
	}

	// 実行
	template<class AddL, class AddR, class RemL, class RemR, class Out>
	void run(AddL add_left, AddR add_right, RemL remove_left, RemR remove_right, Out output) {
		std::sort(order.begin(), order.end(), [&](int a, int b) {
			int ablock = left[a] / block_size;
			int bblock = left[b] / block_size;
			if (ablock != bblock) return ablock < bblock;
			if (ablock & 1) return right[a] < right[b];
			return right[a] > right[b];
			});

		int nl = 0, nr = 0;

		for (int idx : order) {
			while (nl > left[idx]) add_left(--nl);
			while (nr < right[idx]) add_right(nr++);
			while (nl < left[idx]) remove_left(nl++);
			while (nr > right[idx]) remove_right(--nr);

			output(idx);
		}
	}
};

} // namespace kwm_t::algorithm

#endif // KWM_T_ALGORITHM_MO_HPP