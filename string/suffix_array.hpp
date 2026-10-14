/**
 * @file suffix_array.hpp
 * @brief Suffix Array
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/string/suffix_array.hpp
 */
#ifndef KWM_T_STRING_SUFFIX_ARRAY_HPP
#define KWM_T_STRING_SUFFIX_ARRAY_HPP

#include <vector>
#include <string>

/**
 * @brief Suffix Array (SA-IS)
 *
 * SA-IS法によってsuffix arrayを構築する
 *
 * 計算量:
 *   O(N)
 *
 * 使用例:
 *   auto sa = kwm_t::string::suffix_array("banana");
 *
 * verified:
 *   - 自前実装
 */
namespace kwm_t::string {

class SuffixArray {
public:
    explicit SuffixArray(const std::string& s) : S(s) {
        build();
    }

    const std::vector<int>& get() const {
        return SA;
    }

private:
    std::string S;
    std::vector<int> SA;

    static constexpr char SENTINEL = '$';
    static constexpr int KIND = 128;

    void build() {
        // 番兵追加
        S += SENTINEL;

        // char -> int
        std::vector<int> str(S.size());
        for (int i = 0; i < (int)S.size(); ++i) {
            str[i] = S[i] - SENTINEL;
        }

        // SA-IS
        SA = sa_is(str, KIND);

        // 番兵除去
        SA.erase(SA.begin());
    }

    std::vector<int> sa_is(const std::vector<int>& str, int kind) {
        const int n = (int)str.size();

        auto is_s = calc_s_type(str);
        auto is_lms = calc_lms(is_s);

        std::vector<int> lms_pos;
        for (int i = 0; i < n; ++i) {
            if (is_lms[i]) lms_pos.push_back(i);
        }

        auto pseudo_sa = induced_sort(str, lms_pos, is_s, kind);

        std::vector<int> lms_order;
        for (int v : pseudo_sa) {
            if (is_lms[v]) lms_order.push_back(v);
        }

        int rank = 0;
        pseudo_sa[lms_order[0]] = rank;
        if ((int)lms_order.size() > 1) pseudo_sa[lms_order[1]] = ++rank;

        for (int i = 1; i < (int)lms_order.size() - 1; ++i) {
            bool diff = false;
            for (int j = 0; j < n; ++j) {
                int a = lms_order[i] + j;
                int b = lms_order[i + 1] + j;

                if (str[a] != str[b] || is_lms[a] != is_lms[b]) {
                    diff = true;
                    break;
                }
                if (j > 0 && is_lms[a]) break;
            }
            pseudo_sa[lms_order[i + 1]] = diff ? ++rank : rank;
        }

        std::vector<int> reduced;
        reduced.reserve(lms_pos.size());
        for (int i = 0; i < n; ++i) {
            if (is_lms[i]) reduced.push_back(pseudo_sa[i]);
        }

        std::vector<int> lms_sa;
        if (rank + 1 == (int)lms_pos.size()) {
            lms_sa = lms_order;
        } else {
            lms_sa = sa_is(reduced, rank + 1);
            for (auto& x : lms_sa) x = lms_pos[x];
        }

        return induced_sort(str, lms_sa, is_s, kind);
    }

    std::vector<bool> calc_s_type(const std::vector<int>& str) {
        int n = (int)str.size();
        std::vector<bool> is_s(n);
        is_s[n - 1] = true;

        for (int i = n - 2; i >= 0; --i) {
            if (str[i] < str[i + 1]) is_s[i] = true;
            else if (str[i] == str[i + 1] && is_s[i + 1]) is_s[i] = true;
        }
        return is_s;
    }

    std::vector<bool> calc_lms(const std::vector<bool>& is_s) {
        int n = (int)is_s.size();
        std::vector<bool> is_lms(n);

        for (int i = 0; i < n; ++i) {
            if (is_s[i] && (i == 0 || !is_s[i - 1])) {
                is_lms[i] = true;
            }
        }
        return is_lms;
    }

    std::vector<int> induced_sort(
        const std::vector<int>& str,
        const std::vector<int>& lms_pos,
        const std::vector<bool>& is_s,
        int kind
    ) {
        int n = (int)str.size();
        std::vector<int> sa(n);

        std::vector<int> cum(kind + 1);
        for (int c : str) cum[c + 1]++;
        for (int i = 0; i < kind; ++i) cum[i + 1] += cum[i];

        std::vector<int> cnt(kind);

        // Step1
        for (int i = (int)lms_pos.size() - 1; i >= 0; --i) {
            int c = str[lms_pos[i]];
            int pos = cum[c + 1] - cnt[c] - 1;
            sa[pos] = lms_pos[i];
            cnt[c]++;
        }

        // Step2
        std::fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < n; ++i) {
            if (sa[i] == 0) continue;
            if (is_s[sa[i] - 1]) continue;

            int c = str[sa[i] - 1];
            int pos = cum[c] + cnt[c];
            sa[pos] = sa[i] - 1;
            cnt[c]++;
        }

        // Step3
        std::fill(cnt.begin(), cnt.end(), 0);
        for (int i = n - 1; i >= 0; --i) {
            if (sa[i] == 0) continue;
            if (!is_s[sa[i] - 1]) continue;

            int c = str[sa[i] - 1];
            int pos = cum[c + 1] - cnt[c] - 1;
            sa[pos] = sa[i] - 1;
            cnt[c]++;
        }

        return sa;
    }
};

// wrapper
inline std::vector<int> suffix_array(const std::string& s) {
    return SuffixArray(s).get();
}

} // namespace kwm_t::string

#endif // KWM_T_STRING_SUFFIX_ARRAY_HPP