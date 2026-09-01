#ifndef KWM_T_UTILITY_COMPRESSION_HPP
#define KWM_T_UTILITY_COMPRESSION_HPP

#include <vector>
#include <algorithm>
#include <cassert>
#include <utility>

/**
 * @brief 座標圧縮クラス（双方向変換・安全/高速両対応）
 *
 * 典型用途:
 *   - 座標圧縮
 *   - 値 → index 変換
 *   - index → 元の値復元
 *
 * 計算量:
 *   build(): O(N log N)
 *   get / operator(): O(log N)
 *   convert(): O(N log N)
 *
 * @tparam S 圧縮対象の型（int, long long, pairなど）
 *
 * 制約 / 注意:
 *   - build() 後に使用すること
 *   - operator() は「存在する前提」
 *   - get() は存在しない場合 -1
 *
 * 使用例:
 *   Compression<int> comp;
 *   for (auto x : v) comp.add(x);
 *   comp.build();
 *   comp.convert(v.begin(), v.end());
 *
 * verified:
 * https://atcoder.jp/contests/awc0027/submissions/74186257
 */
namespace kwm_t::utility {

template <class S>
struct Compression : private std::vector<S> {
	using VS = std::vector<S>;
	using typename VS::iterator;
	using typename VS::const_iterator;

private:
	bool built = false;

public:
	using VS::VS;

	Compression() = default;
	Compression(const std::vector<S>& v) : VS(v) {}
	Compression(std::vector<S>&& v) : VS(std::move(v)) {}

	// 要素追加
	void add(const S& x) {
		this->push_back(x);
		built = false;
	}

	// 構築
	void build() {
		std::sort(this->begin(), this->end());
		this->erase(std::unique(this->begin(), this->end()), this->end());
		built = true;
	}

	void build_if_needed() {
		if (!built) build();
	}

	// 値 -> index（高速版：存在前提）
	int operator()(const S& x) const {
		assert(built);
		return std::lower_bound(this->begin(), this->end(), x) - this->begin();
	}

	// 値 -> index（安全版：存在しなければ -1）
	int get(const S& x) const {
		assert(built);
		auto it = std::lower_bound(this->begin(), this->end(), x);
		if (it == this->end() || *it != x) return -1;
		return it - this->begin();
	}

	// index -> 元の値
	const S& rev(int i) const {
		assert(built);
		return (*this)[i];
	}

	// in-place 変換
	template <class It>
	void convert_inplace(It first, It last) const {
		assert(built);
		for (; first != last; ++first) {
			*first = (*this)(*first);
		}
	}

	// 別配列に出力
	template <class It, class Out>
	void convert(It first, It last, Out out) const {
		assert(built);
		for (; first != last; ++first, ++out) {
			*out = (*this)(*first);
		}
	}

	// サイズ
	int size() const {
		return VS::size();
	}

	// クリア
	void clear() {
		VS::clear();
		built = false;
	}

	// lower_bound の index(x未満の数)
	int lower_bound_index(const S& x) const {
		assert(built);
		return std::lower_bound(this->begin(), this->end(), x) - this->begin();
	}

	// upper_bound の index(x以下の数)
	int upper_bound_index(const S& x) const {
		assert(built);
		return std::upper_bound(this->begin(), this->end(), x) - this->begin();
	}
};


// 関数版（軽量）
template<typename T>
struct CompressResult {
	std::vector<int> compressed;
	std::vector<T> values;
};

template<typename T>
CompressResult<T> compression(std::vector<T> v) {
	std::vector<T> xs = v;
	std::sort(xs.begin(), xs.end());
	xs.erase(std::unique(xs.begin(), xs.end()), xs.end());

	std::vector<int> res(v.size());
	for (int i = 0; i < (int)v.size(); i++) {
		res[i] = std::lower_bound(xs.begin(), xs.end(), v[i]) - xs.begin();
	}

	return { res, xs };
}

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_COMPRESSION_HPP