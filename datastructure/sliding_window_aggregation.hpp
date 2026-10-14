/**
 * @file sliding_window_aggregation.hpp
 * @brief Sliding Window Aggregation
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/sliding_window_aggregation.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_SLIDING_WINDOW_AGGREGATION_HPP
#define KWM_T_DATA_STRUCTURE_SLIDING_WINDOW_AGGREGATION_HPP

#include <stack>
#include <cassert>

namespace kwm_t::data_structure {

/**
 * @brief スライディングウィンドウ集約（SWAG）
 *
 * @details
 *  モノイド (T, op) に対して、キュー上の要素の総積を
 *  amortized O(1) で管理するデータ構造。
 *
 *  2つのスタック（front, back）を用いて、
 *  push / pop / fold_all を効率的に処理する。
 *
 *  非可換モノイドにも対応可能。
 *
 *  ※ op の順序に注意:
 *     fold_all() = op(front側, back側)
 *
 * 典型用途:
 *   - スライディングウィンドウの区間和 / min / max / gcd
 *   - 尺取り法 + 集約
 *   - deque最適化の一般化
 *
 * 計算量:
 *   - push: amortized O(1)
 *   - pop:  amortized O(1)
 *   - fold_all: O(1)
 *
 * @tparam T
 *   モノイドの要素型
 * @tparam Op
 *   二項演算（結合則を満たす必要あり）
 *
 * 制約 / 注意:
 *   - 単位元は内部では持たないため、空状態で fold_all() は不可
 *   - 非可換の場合は op の順序に注意
 *
 * 使用例:
 *   struct Min {
 *       int operator()(int a, int b) const { return std::min(a, b); }
 *   };
 *
 *   kwm_t::data_structure::SlidingWindowAggregation<int, Min> swag;
 *
 *   swag.push(3);
 *   swag.push(1);
 *   swag.push(4);
 *
 *   int x = swag.fold_all(); // = 1
 *
 *   swag.pop();
 *
 * verified:
 *   - AtCoder Library Practice Contest (SWAG系)
 */
template <class T, class Op>
struct SlidingWindowAggregation {
private:
	struct Node {
		T val;
		T agg;
		Node(const T& v, const T& a) : val(v), agg(a) {}
	};

	Op op;
	std::stack<Node> front, back;

	void rebuild() {
		// back → front に移動
		while (!back.empty()) {
			T v = back.top().val;
			back.pop();
			if (front.empty()) {
				front.emplace(v, v);
			} else {
				front.emplace(v, op(v, front.top().agg));
			}
		}
	}

public:
	SlidingWindowAggregation() = default;
	explicit SlidingWindowAggregation(const Op& op_) : op(op_) {}

	bool empty() const {
		return front.empty() && back.empty();
	}

	std::size_t size() const {
		return front.size() + back.size();
	}

	void clear() {
		while (!front.empty()) front.pop();
		while (!back.empty()) back.pop();
	}

	// 末尾に追加
	void push(const T& x) {
		if (back.empty()) {
			back.emplace(x, x);
		} else {
			back.emplace(x, op(back.top().agg, x));
		}
	}

	// 先頭を削除
	void pop() {
		assert(!empty());
		if (front.empty()) rebuild();
		front.pop();
	}

	// 全体の集約値
	T fold_all() const {
		assert(!empty());
		if (front.empty()) return back.top().agg;
		if (back.empty())  return front.top().agg;
		return op(front.top().agg, back.top().agg);
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_SLIDING_WINDOW_AGGREGATION_HPP