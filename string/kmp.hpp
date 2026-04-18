#ifndef KWM_T_STRING_KMP_HPP
#define KWM_T_STRING_KMP_HPP

#include <vector>

/**
 * @brief KMP (Knuth-Morris-Pratt) 法（最適化版）
 *
 * パターン列の prefix function（failure function）を構築し、
 * テキスト中の出現位置を高速に列挙する。
 * 本実装は next 配列の最適化を含む。
 *
 * 典型用途:
 *   - 文字列検索（完全一致）
 *   - 部分文字列の出現位置列挙
 *
 * 計算量:
 *   - 構築: O(|pattern|)
 *   - 検索: O(|text|)
 *
 * @tparam T
 *   - size(), operator[] を持ち、== で比較可能なコンテナ
 *   - 例: std::string, std::vector<int>
 *
 * @param pattern 検索したいパターン
 *
 * 制約 / 注意:
 *   - pattern は空でないことを想定
 *
 * 使用例:
 *   kwm_t::string::KMP<std::string> kmp("aba");
 *   auto res = kmp.search("ababa");
 *   // res = {0, 2}
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0045/submissions/74809117
 */
namespace kwm_t::string {

template <typename T>
class KMP {
public:
	explicit KMP(const T& pattern) : pattern(pattern) {
		build();
	}

	// text 内の一致開始位置をすべて返す
	std::vector<int> search(const T& text) const {
		std::vector<int> res;
		int j = 0;
		for (int i = 0; i < (int)text.size(); ++i) {
			while (j != -1 && pattern[j] != text[i]) j = next[j];
			++j;
			if (j == n) {
				res.push_back(i - j + 1);
				j = next[j];
			}
		}
		return res;
	}

	// failure function を取得（デバッグ・応用用）
	const std::vector<int>& get_next() const {
		return next;
	}

private:
	int n;
	T pattern;
	std::vector<int> next;

	void build() {
		n = (int)pattern.size();
		next.assign(n + 1, 0);

		next[0] = -1;
		int j = -1;

		for (int i = 0; i < n; ++i) {
			while (j != -1 && pattern[j] != pattern[i]) j = next[j];
			++j;
			if (i + 1 < n && pattern[i + 1] == pattern[j]) {
				next[i + 1] = next[j]; // 最適化
			}
			else {
				next[i + 1] = j;
			}
		}
	}
};

} // namespace kwm_t::string

#endif // KWM_T_STRING_KMP_HPP