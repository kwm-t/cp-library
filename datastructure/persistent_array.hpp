#ifndef KWM_T_DATA_STRUCTURE_PERSISTENT_ARRAY_HPP
#define KWM_T_DATA_STRUCTURE_PERSISTENT_ARRAY_HPP

#include <cassert>
#include <memory>
#include <vector>

namespace kwm_t::data_structure {

/**
 * @brief Persistent Array
 *
 * 配列の各要素への代入を永続化して管理するデータ構造。
 *
 * set() は元の配列を変更せず、変更後の新しい PersistentArray を返す。
 * そのため、過去のバージョンもそのまま保持して利用できる。
 *
 * 内部では B 分木を用い、添字 k の B 進表現を利用して
 * 根から葉までを辿る。
 *
 * 例えば B = 2 の場合、添字の2進表現に対応する経路を持つ。
 *
 * --------------------------------------------
 * 計算量
 * --------------------------------------------
 *
 * 配列の添字を k とする。
 *
 * get(k):
 *   O(log_B(k + 1))
 *
 * set(k, x):
 *   O(log_B(k + 1))
 *
 * set() では根から変更箇所までの経路上の Node だけをコピーする。
 * したがって、1 回の set() で新しく生成される Node 数も
 *
 *   O(log_B(k + 1))
 *
 * である。
 *
 * B が定数なら、いずれも O(log k) と考えられる。
 *
 * --------------------------------------------
 * 空間計算量
 * --------------------------------------------
 *
 * 1 回の set() につき
 *
 *   O(log_B(k + 1))
 *
 * 個の Node が新しく生成される。
 *
 * 初期配列をコンストラクタから構築した場合、
 * 現在の実装では各要素について set() を行うため、
 *
 *   O(N log_B N)
 *
 * の Node が生成される。
 *
 * M 回の set() を行った場合の追加メモリは
 *
 *   O(M log_B N)
 *
 * である。
 *
 * 各 Node は shared_ptr によって管理されるため、
 * あるバージョンを破棄しても、他のバージョンから
 * 参照されている Node は破棄されない。
 *
 * --------------------------------------------
 * B について
 * --------------------------------------------
 *
 * B を大きくすると木の高さは小さくなる一方、
 * 各 Node が持つ子ポインタの数が増える。
 *
 * 1 Node あたりのメモリ使用量は O(B)。
 *
 * そのため、B は速度とメモリのトレードオフになる。
 *
 * 競技プログラミングでは B = 2 ～ 32 程度を用途に応じて
 * 選択すればよい。
 *
 * @tparam T 配列の要素型
 * @tparam B 各 Node の分岐数。2以上。
 *
 * @param k 配列の添字。0以上。
 *
 * 制約 / 注意:
 * - k は 0 以上でなければならない。
 * - get() で一度も値を設定していない要素を参照した場合、
 *   T{} を返す。
 * - set() は元の配列を変更しない。
 * - 各バージョンは共通する Node を共有する。
 *
 * 使用例:
 *
 *   kwm_t::data_structure::PersistentArray<int> a;
 *
 *   auto b = a.set(3, 10);
 *   auto c = b.set(5, 20);
 *
 *   assert(a.get(3) == 0);
 *   assert(b.get(3) == 10);
 *   assert(b.get(5) == 0);
 *   assert(c.get(3) == 10);
 *   assert(c.get(5) == 20);
 *
 * verified:
 *   未verified
 */
template <typename T, int B = 16>
class PersistentArray {
	static_assert(B >= 2, "B must be at least 2.");

	struct Node;
	using node_ptr = std::shared_ptr<Node>;

	struct Node {
		T val{};
		node_ptr ch[B]{};
	};

	node_ptr root = nullptr;

	explicit PersistentArray(const node_ptr& root) : root(root) {}

	T get(const node_ptr& t, int k) const {
		if (!t) return T{};

		if (k == 0) {
			return t->val;
		}

		return get(t->ch[k % B], k / B);
	}

	node_ptr set(const node_ptr& t, int k, const T& x) const {
		node_ptr res;

		if (t) {
			res = std::make_shared<Node>(*t);
		}
		else {
			res = std::make_shared<Node>();
		}

		if (k == 0) {
			res->val = x;
		}
		else {
			const int p = k % B;
			res->ch[p] = set(res->ch[p], k / B, x);
		}

		return res;
	}

public:
	/**
	 * @brief 空の Persistent Array を構築する。
	 *
	 * 計算量:
	 *   O(1)
	 *
	 * 空間計算量:
	 *   O(1)
	 */
	PersistentArray() = default;

	/**
	 * @brief vector から Persistent Array を構築する。
	 *
	 * 各要素について set() を行う実装になっている。
	 *
	 * 計算量:
	 *   O(N log_B N)
	 *
	 * 空間計算量:
	 *   O(N log_B N)
	 *
	 * @param v 初期値。
	 */
	explicit PersistentArray(const std::vector<T>& v) {
		for (int i = 0; i < static_cast<int>(v.size()); ++i) {
			root = set(root, i, v[i]);
		}
	}

	/**
	 * @brief k 番目の要素を取得する。
	 *
	 * 計算量:
	 *   O(log_B(k + 1))
	 *
	 * @param k 添字。
	 * @return k 番目の要素。
	 *
	 * 注意:
	 *   まだ一度も set() されていない要素については T{} を返す。
	 */
	T get(int k) const {
		assert(k >= 0);
		return get(root, k);
	}

	/**
	 * @brief k 番目の要素を x に変更した新しいバージョンを返す。
	 *
	 * 元の PersistentArray は変更されない。
	 *
	 * 計算量:
	 *   O(log_B(k + 1))
	 *
	 * 追加空間:
	 *   O(log_B(k + 1))
	 *
	 * @param k 添字。
	 * @param x 新しい値。
	 * @return k 番目を x に変更した新しい PersistentArray。
	 */
	PersistentArray set(int k, const T& x) const {
		assert(k >= 0);
		return PersistentArray(set(root, k, x));
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_PERSISTENT_ARRAY_HPP