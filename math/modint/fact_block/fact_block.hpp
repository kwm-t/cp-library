#ifndef KWM_T_MATH_MODINT_FACT_BLOCK_GEN_HPP
#define KWM_T_MATH_MODINT_FACT_BLOCK_GEN_HPP

#include <iostream>

/**
 * @brief factorial ブロックテーブル生成ツール
 *
 * 典型用途:
 *   fact_block.hpp 用の配列を生成してコピペする
 *
 * 計算量:
 *   O(mod)
 *
 * @tparam Mint modint型
 *
 * 使用例:
 *   create_fact_block_table<mint, 1000000>();
 *
 * 出力:
 *   static std::vector<Mint> FACT_BLOCK = {...};
 *
 * 注意:
 *   - 標準出力に出るのでリダイレクト推奨
 *   - mod は Mint::mod() を使用
 */
namespace kwm_t::math::modint {

	template <class Mint, int BLOCK_SIZE>
	void create_fact_block_table() {
		const int mod = Mint::mod();
		const int sz = (mod + BLOCK_SIZE - 1) / BLOCK_SIZE;

		std::cout << "// auto-generated factorial block table\n";
		std::cout << "static std::vector<Mint> FACT_BLOCK = {\n";

		Mint val = 1;
		for (int i = 0; i <= sz * BLOCK_SIZE; ++i) {
			if (i != 0) val *= i;

			if (i % BLOCK_SIZE == 0) {
				std::cout << "    " << val.val();
				if (i != sz * BLOCK_SIZE) std::cout << ",";
				std::cout << "\n";
			}
		}

		std::cout << "};\n";
	}

} // namespace kwm_t::math::modint

#endif // KWM_T_MATH_MODINT_FACT_BLOCK_GEN_HPP