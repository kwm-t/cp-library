/**
 * @file top_k.hpp
 * @brief 上位 K 要素を管理するデータ構造
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/top_k.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_TOP_K_HPP
#define KWM_T_DATA_STRUCTURE_TOP_K_HPP

#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>

namespace kwm_t::data_structure {

/**
 * @brief 上位K個管理（小K向け・配列版）
 *
 * @details
 *   常に「良い順にK個」を保持する。
 *   update は O(K)。
 *
 *   Kが小さい（～50くらい）場合はこちらが高速。
 *
 * @tparam T
 * @tparam Compare
 *   「良いかどうか」を判定する比較関数
 *   comp(a, b) == true → a の方が良い
 *
 * 使用例:
 *   TopKSmall<int, std::less<int>> tk(3, INF);
 *   tk.update(5);
 */
template <class T, class Compare = std::less<T>>
struct TopKSmall {
private:
	int K;
	std::vector<T> v;
	Compare comp;

public:
	TopKSmall() = default;

	TopKSmall(int K_, const T& init, Compare comp_ = Compare())
		: K(K_), v(K_, init), comp(comp_) {}

	// 更新（採用されたら true）
	bool update(const T& x) {
		for (int i = 0; i < K; i++) {
			if (comp(x, v[i])) {
				for (int j = K - 1; j > i; j--) v[j] = v[j - 1];
				v[i] = x;
				return true;
			}
		}
		return false;
	}

	const std::vector<T>& data() const {
		return v;
	}

	void clear(const T& init) {
		std::fill(v.begin(), v.end(), init);
	}
};

/**
 * @brief 上位K個管理（heap版）
 *
 * @details
 *   priority_queue を使って上位K個を管理。
 *   update は O(log K)。
 *
 *   Kが大きい場合はこちら。
 *
 * @tparam T
 * @tparam Compare
 *   「良いかどうか」を判定する比較関数
 *   comp(a, b) == true → a の方が良い
 *
 * 注意:
 *   pq.top() は「最も悪い要素」になるように Compare を設計する
 *
 * 使用例:
 *   TopKHeap<int, std::less<int>> tk(3);
 */
template <class T, class Compare = std::less<T>>
struct TopKHeap {
private:
	int K;
	Compare comp;

	// 最も悪い要素が top に来るようにする
	struct PQCmp {
		Compare comp;
		PQCmp(Compare c) : comp(c) {}
		bool operator()(const T& a, const T& b) const {
			return comp(b, a); // reverse
		}
	};

	std::priority_queue<T, std::vector<T>, PQCmp> pq;

public:
	TopKHeap() = default;

	explicit TopKHeap(int K_, Compare comp_ = Compare())
		: K(K_), comp(comp_), pq(PQCmp(comp_)) {}

	bool update(const T& x) {
		if ((int)pq.size() < K) {
			pq.push(x);
			return true;
		}
		if (comp(x, pq.top())) {
			pq.pop();
			pq.push(x);
			return true;
		}
		return false;
	}

	std::size_t size() const {
		return pq.size();
	}

	bool empty() const {
		return pq.empty();
	}

	void clear() {
		while (!pq.empty()) pq.pop();
	}

	// ソート済みで取得（良い順）
	std::vector<T> sorted() const {
		auto tmp = pq;
		std::vector<T> res;
		while (!tmp.empty()) {
			res.push_back(tmp.top());
			tmp.pop();
		}
		std::sort(res.begin(), res.end(), comp);
		return res;
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_TOP_K_HPP