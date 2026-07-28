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
 *   区間種類数
 *   https://atcoder.jp/contests/awc0122/submissions/77921512
 *	 一般の場合
 *	 https://atcoder.jp/contests/abc463/submissions/76868142
 */
namespace kwm_t::algorithm {

class Mo {
private:
	int q;
	int block_size;
	int min_coord;
	int default_l, default_r;

	std::vector<int> left, right;
	std::vector<int> order;

public:
	// 0 <= l <= r < n
	Mo(int n_, const std::vector<std::pair<int, int>>& queries) : Mo(0, n_, queries) {}

	Mo(int n_, int q_) : Mo(0, n_, q_) {}

	// l_bound <= l,r <= r_bound
	Mo(int l_bound, int r_bound, const std::vector<std::pair<int, int>>& queries)
		: q(queries.size()), min_coord(l_bound), default_l(l_bound), default_r(l_bound) {

		init(1LL * r_bound - l_bound + 1, q);
		query(queries);
	}

	Mo(int l_bound, int r_bound, int q_)
		: q(q_), min_coord(l_bound), default_l(l_bound), default_r(l_bound) {

		init(1LL * r_bound - l_bound + 1, q);
	}

	// クエリ設定
	void query(const std::vector<int>& l, const std::vector<int>& r) {
		left = l;
		right = r;
	}

	// クエリ設定
	void query(const std::vector<std::pair<int, int>>& queries) {
		left.clear();
		right.clear();
		left.reserve(queries.size());
		right.reserve(queries.size());

		for (auto [l, r] : queries) {
			left.push_back(l);
			right.push_back(r);
		}
	}

private:
	void init(long long coord_size, int q_) {
		block_size = std::max(1, int(double(coord_size) / std::sqrt(q_)));
		order.resize(q_);
		std::iota(order.begin(), order.end(), 0);
	}

	template<class AddL, class AddR, class RemL, class RemR, class Out>
	void run_impl(AddL add_left, AddR add_right, RemL remove_left, RemR remove_right, Out output, int nl, int nr) {

		std::sort(order.begin(), order.end(), [&](int a, int b) {
			int ablock = (left[a] - min_coord) / block_size;
			int bblock = (left[b] - min_coord) / block_size;

			if (ablock != bblock) return ablock < bblock;
			if (ablock & 1) return right[a] < right[b];
			return right[a] > right[b];
		});

		for (int idx : order) {
			while (nl > left[idx]) add_left(--nl);
			while (nr < right[idx]) add_right(nr++);
			while (nl < left[idx]) remove_left(nl++);
			while (nr > right[idx]) remove_right(--nr);

			output(idx);
		}
	}

public:
	// デフォルト開始位置
	template<class AddL, class AddR, class RemL, class RemR, class Out>
	void run(AddL add_left, AddR add_right, RemL remove_left, RemR remove_right, Out output) {
		run_impl(add_left, add_right, remove_left, remove_right, output, default_l, default_r);
	}
	template<class Add, class Rem, class Out>
	void run(Add add, Rem remove, Out output) {
		run_impl(add, add, remove, remove, output, default_l, default_r);
	}
	// 任意開始位置
	template<class AddL, class AddR, class RemL, class RemR, class Out>
	void run(AddL add_left, AddR add_right, RemL remove_left, RemR remove_right, Out output, int init_l, int init_r) {
		run_impl(add_left, add_right, remove_left, remove_right, output, init_l, init_r);
	}
	template<class Add, class Rem, class Out>
	void run(Add add, Rem remove, Out output, int init_l, int init_r) {
		run_impl(add, add, remove, remove, output, init_l, init_r);
	}
};

} // namespace kwm_t::algorithm

#endif // KWM_T_ALGORITHM_MO_HPP