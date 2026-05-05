#ifndef KWM_T_DATASTRUCTURE_RANGE_ADD_AP_HPP
#define KWM_T_DATASTRUCTURE_RANGE_ADD_AP_HPP

#include <vector>
#include <algorithm>

namespace kwm_t::datastructure {
/**
 * @brief 区間に等差数列を加算（オフライン）
 *
 * 初項 x、公差 y の等差数列を区間 [l, r) に加算する。
 * 複数回の set() の後、build() を呼ぶことで最終配列を構築する。
 *
 * 内部的には「一次関数 (a*i + b) の差分」を2本で管理。
 *
 * 典型用途:
 *   ・区間に x, x+y, x+2y,... を加算
 *   ・imos法の拡張（等差版）
 *
 * 計算量:
 *   set(): O(1)
 *   build(): O(N)
 *
 * @tparam T 加算可能な型（int, long long, modint など）
 *
 * @param sz 配列サイズ
 *
 * 制約 / 注意:
 *   ・クエリはオフライン（build後は再利用不可）
 *   ・範囲外や r < l は内部で吸収
 *
 * 使用例:
 *   RangeAddAP<ll> ra(n);
 *   ra.set(1, 2, 0, 5); // [1,3,5,7,9]
 *   ra.build();
 *
 * verified:
 *   https://atcoder.jp/contests/awc0062/submissions/75532999
 */
template<typename T>
struct RangeAddAP {
private:
    int sz;
    std::vector<T> diff1; // 一次係数（y）
    std::vector<T> diff0; // 定数補正

    // 範囲・数列の正規化
    void normalize(T& x, T& y, int& l, int& r) {
        if (r < l) {
            x += (l - r - 1) * y;
            y = -y;
            std::swap(l, r);
            ++l; ++r;
        }
        if (r > sz) r = sz;
        if (l < 0) {
            x += (-l) * y;
            l = 0;
        }
    }

public:
    RangeAddAP(int n) : sz(n), diff1(n + 1), diff0(n + 1) {}

    /**
     * @brief 等差数列を加算
     *
     * @param x 初項
     * @param y 公差
     * @param l 左端（含む）
     * @param r 右端（含まない）
     */
    void set(T x, T y, int l, int r) {
        normalize(x, y, l, r);
        if (l >= r) return;

        // y の差分
        diff1[l] += y;
        diff1[r] -= y;

        // 定数項補正
        diff0[l] += x - y;
        diff0[r] -= x - y;
        diff0[r] -= y * (r - l);
    }

    /**
     * @brief 構築
     *
     * @return 最終配列
     */
    std::vector<T> build() {
        std::vector<T> res = diff1;

        // diff1 prefix
        for (int i = 0; i < sz; ++i) {
            res[i + 1] += res[i];
        }

        // diff0 加算
        for (int i = 0; i <= sz; ++i) {
            res[i] += diff0[i];
        }

        // 再度 prefix
        for (int i = 0; i < sz; ++i) {
            res[i + 1] += res[i];
        }

        res.pop_back(); // サイズ sz に
        return res;
    }
};

} // namespace kwm_t::datastructure

#endif // KWM_T_DATASTRUCTURE_RANGE_ADD_AP_HPP