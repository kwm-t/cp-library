#ifndef KWM_T_DATA_STRUCTURE_BLOCK_SEQUENCE_HPP
#define KWM_T_DATA_STRUCTURE_BLOCK_SEQUENCE_HPP

#include <cassert>
#include <deque>
#include <utility>

namespace kwm_t::data_structure {

/**
 * @brief
 * 要素列を複数のブロックに分割して管理するシーケンス。
 *
 * 各ブロックのサイズを BLOCK_SIZE 程度に保つことで、
 * 任意位置への挿入時に移動する要素数を抑える。
 *
 * ブロックを先頭から走査して挿入位置を含むブロックを特定し、
 * そのブロック内に要素を挿入する。
 *
 * ブロックのサイズが 2 * BLOCK_SIZE に達すると、
 * 2 つのブロックに分割する。
 *
 * @tparam T 要素の型。
 *
 * @note
 * insert() の位置 k は 0-indexed で、
 * 0 <= k <= size() を満たす必要がある。
 *
 * @note
 * BLOCK_SIZE は正の整数である必要がある。
 *
 * 計算量:
 *   insert:
 *     O(B + BLOCK_SIZE)
 *
 *   push_back:
 *     O(BLOCK_SIZE)
 *     ただし通常は O(1) 償却程度。
 *
 *   front:
 *     O(1)
 *
 *   pop_front:
 *     O(1)
 *
 *   size:
 *     O(1)
 *
 *   empty:
 *     O(1)
 *
 * ここで B はブロック数。
 *
 * N 個の要素を BLOCK_SIZE = Θ(sqrt(N)) 程度で管理する場合、
 * ブロック数 B = O(sqrt(N)) となるため、
 * insert() は概ね O(sqrt(N)) で実行できる。
 *
 * 使用例:
 * @code
 * kwm_t::data_structure::BlockSequence<int> seq(100);
 *
 * seq.push_back(1);
 * seq.push_back(3);
 * seq.insert(1, 2);
 *
 * // [1, 2, 3]
 * int x = seq.front();
 * seq.pop_front();
 * @endcode
 *
 * verified:
 *   https://atcoder.jp/contests/awc0118/submissions/77745249
 */
template <class T>
class BlockSequence {
private:
	std::deque<std::deque<T>> blocks;
	const int BLOCK_SIZE;
	int sz = 0;

public:
	/**
	 * @brief コンストラクタ。
	 *
	 * @param block_size ブロックの基準サイズ。
	 *
	 * 制約:
	 *   block_size > 0
	 *
	 * 計算量:
	 *   O(1)
	 */
	explicit BlockSequence(int block_size)
		: BLOCK_SIZE(block_size) {
		assert(BLOCK_SIZE > 0);
	}

	/**
	 * @brief 要素数を返す。
	 *
	 * @return 現在の要素数。
	 *
	 * 計算量:
	 *   O(1)
	 */
	[[nodiscard]] int size() const {
		return sz;
	}

	/**
	 * @brief 空かどうかを返す。
	 *
	 * @return 空なら true。
	 *
	 * 計算量:
	 *   O(1)
	 */
	[[nodiscard]] bool empty() const {
		return sz == 0;
	}

	/**
	 * @brief k 番目の位置に要素を挿入する。
	 *
	 * k は 0-indexed。
	 * k == size() の場合は末尾に挿入する。
	 *
	 * @param k 挿入位置。
	 * @param value 挿入する値。
	 *
	 * 制約:
	 *   0 <= k <= size()
	 *
	 * 計算量:
	 *   O(B + BLOCK_SIZE)
	 *
	 * B はブロック数。
	 */
	void insert(int k, const T& value) {
		assert(0 <= k && k <= sz);

		if (blocks.empty()) {
			blocks.emplace_back();
			blocks.back().push_back(value);
			++sz;
			return;
		}

		for (int i = 0; i < static_cast<int>(blocks.size()); ++i) {
			const int block_size =
				static_cast<int>(blocks[i].size());

			if (k > block_size) {
				k -= block_size;
				continue;
			}

			blocks[i].insert(
				blocks[i].begin() + k,
				value
			);

			++sz;

			if (static_cast<int>(blocks[i].size()) == 2 * BLOCK_SIZE) {
				split_block(i);
			}

			return;
		}

		// k == size() の場合も必ず上で処理される。
		assert(false);
	}

	/**
	 * @brief 末尾に要素を追加する。
	 *
	 * @param value 追加する値。
	 *
	 * 計算量:
	 *   通常 O(1)。
	 *
	 * ブロック分割が発生した場合は
	 * O(BLOCK_SIZE)。
	 */
	void push_back(const T& value) {
		if (blocks.empty()) {
			blocks.emplace_back();
		}

		blocks.back().push_back(value);
		++sz;

		if (static_cast<int>(blocks.back().size()) == 2 * BLOCK_SIZE) {
			split_block(static_cast<int>(blocks.size()) - 1);
		}
	}

	/**
	 * @brief 先頭要素を返す。
	 *
	 * @return 先頭要素への参照。
	 *
	 * 制約:
	 *   空でないこと。
	 *
	 * 計算量:
	 *   O(1)
	 */
	T& front() {
		assert(!empty());
		return blocks.front().front();
	}

	/**
	 * @brief 先頭要素を返す。
	 *
	 * @return 先頭要素への const 参照。
	 *
	 * 制約:
	 *   空でないこと。
	 *
	 * 計算量:
	 *   O(1)
	 */
	const T& front() const {
		assert(!empty());
		return blocks.front().front();
	}

	/**
	 * @brief 先頭要素を削除する。
	 *
	 * 制約:
	 *   空でないこと。
	 *
	 * 計算量:
	 *   O(1)
	 */
	void pop_front() {
		assert(!empty());

		blocks.front().pop_front();
		--sz;

		if (blocks.front().empty()) {
			blocks.pop_front();
		}
	}

private:
	/**
	 * @brief 指定したブロックを 2 つに分割する。
	 *
	 * block_index 番目のブロックを、
	 * 前半と後半の 2 ブロックに分割する。
	 *
	 * @param block_index 分割対象のブロック番号。
	 *
	 * 計算量:
	 *   O(BLOCK_SIZE)
	 */
	void split_block(int block_index) {
		auto& block = blocks[block_index];

		std::deque<T> right(
			block.begin() + BLOCK_SIZE,
			block.end()
		);

		block.resize(BLOCK_SIZE);

		blocks.insert(
			blocks.begin() + block_index + 1,
			std::move(right)
		);
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_BLOCK_SEQUENCE_HPP