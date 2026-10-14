/**
 * @file fact_block_gen.hpp
 * @brief 階乗ブロックの生成
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/modint/fact_block/fact_block_gen.hpp
 */
#ifndef KWM_T_MATH_MODINT_FACT_BLOCK_HPP
#define KWM_T_MATH_MODINT_FACT_BLOCK_HPP

#include <vector>
#include <cassert>

/**
 * @brief ブロック分割 factorial（n! mod p）
 *
 * 典型用途:
 *   大きな n の factorial
 *
 * 計算量:
 *   O(BLOCK_SIZE)
 *
 * @tparam Mint modint型
 * @tparam BLOCK_SIZE ブロックサイズ
 *
 * 制約 / 注意:
 *   - mod は素数
 *   - n >= mod のとき 0
 *   - fact_block.size() == ceil(mod / BLOCK_SIZE)
 *
 * 使用例:
 *   FactBlock<mint, 1000000> fb(table);
 *   auto x = fb.calc(n);
 */
namespace kwm_t::math::modint {

	template <class Mint, int BLOCK_SIZE>
	class FactBlock {
	private:
		const std::vector<Mint>* fact_block;

	public:
		FactBlock() = default;

		explicit FactBlock(const std::vector<Mint>& table)
			: fact_block(&table) {}

		void set_table(const std::vector<Mint>& table) {
			fact_block = &table;
		}

		Mint calc(long long n) const {
			assert(fact_block != nullptr);

			if (n >= Mint::mod()) return 0;

			const auto& fb = *fact_block;

			int idx = n / BLOCK_SIZE;
			assert(idx < (int)fb.size());

			Mint res = fb[idx];

			long long base = (long long)idx * BLOCK_SIZE;
			for (long long i = base + 1; i <= n; ++i) {
				res *= i;
			}
			return res;
		}
	};

} // namespace kwm_t::math::modint

#endif // KWM_T_MATH_MODINT_FACT_BLOCK_HPP