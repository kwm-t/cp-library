/**
 * @file dsu_wrapper.hpp
 * @brief AtCoder Library の DSU ラッパー
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/acl_wrapper/dsu_wrapper.hpp
 */
#ifndef KWM_T_ACLWRAPPER_DSU_WRAPPER_HPP
#define KWM_T_ACLWRAPPER_DSU_WRAPPER_HPP

#include <vector>
#include <cassert>
#include <utility>
#include <atcoder/dsu>

/**
 * @brief データ付きDSU (Union-Find)
 *
 * 各連結成分に対してデータを持ち、merge時にそれらを統合する。
 *
 * @tparam Data   各頂点(成分)に持たせるデータ型
 * @tparam Merge  データをマージする関数オブジェクト
 *
 * 典型用途:
 *   - 連結成分ごとのサイズ/和/最大値などの管理
 *   - union時に情報を集約したいとき
 *
 * 計算量:
 *   - merge / leader / same : amortized O(α(N))
 *
 * 制約 / 注意:
 *   - Merge は以下の形である必要がある:
 *       Data operator()(const Data&, const Data&)
 *   - merge後、非代表元のデータは使用されない
 *
 * 使用例:
 *   auto merge = [](int a, int b) { return a + b; };
 *   aclwrapper::DsuWrapper<int, decltype(merge)> dsu(n, init, merge);
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0132/submissions/78301971
 */

namespace kwm_t::aclwrapper {

template<typename Data, class Merge>
struct DsuWrapper {
public:
	/**
	 * @brief コンストラクタ
	 * @param n 頂点数
	 * @param init_data 各頂点の初期データ (size == n)
	 * @param merge_fn データ統合関数
	 */
	DsuWrapper(int n, std::vector<Data> init_data, Merge merge_fn)
		: dsu(n), data(std::move(init_data)), merge_fn(merge_fn) {
		assert((int)data.size() == n);
	}

	/**
	 * @brief a と b をマージする
	 * @return 新しいリーダー
	 */
	int merge(int a, int b) {
		a = dsu.leader(a);
		b = dsu.leader(b);
		if (a == b) return a;

		int l = dsu.merge(a, b);
		data[l] = merge_fn(data[a], data[b]);
		return l;
	}

	/**
	 * @brief a の属する連結成分のデータ取得
	 */
	const Data& get(int a) {
		return data[dsu.leader(a)];
	}

	/**
	 * @brief a と b が同一連結成分か
	 */
	bool same(int a, int b) {
		return dsu.same(a, b);
	}

	/**
	 * @brief a の属する連結成分の代表元
	 */
	int leader(int a) {
		return dsu.leader(a);
	}

	/**
	 * @brief a の属する連結成分のサイズ
	 */
	int size(int a) {
		return dsu.size(a);
	}

	/**
	 * @brief 全グループ取得 (ACLそのまま)
	 */
	std::vector<std::vector<int>> groups() {
		return dsu.groups();
	}

private:
	atcoder::dsu dsu;
	std::vector<Data> data;
	Merge merge_fn;
};

} // namespace kwm_t::aclwrapper

#endif // KWM_T_ACLWRAPPER_DSU_WRAPPER_HPP