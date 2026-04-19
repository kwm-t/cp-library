#ifndef KWM_T_MATH_BSGS_HPP
#define KWM_T_MATH_BSGS_HPP

// ============================================
//  Library Template (Instruction-Based)
//  BabyStepGiantStep
// ============================================
//
// ■ 概要
//   離散対数問題:
//     op(x, g, g, ..., g) = target を解く
//
//   一般化BSGS:
//     - 群演算 op
//     - 単位元 e
//     - 逆元 inv
//
// ■ 典型用途
//   - a^x ≡ b (mod p)
//   - 行列の冪の逆問題
//   - 状態遷移のログ問題
//
// ■ 計算量
//   O(√n)
//
// ■ 制約 / 注意
//   - S は hashable である必要あり（unordered_map使用）
//   - op は結合的であること
//   - inv は逆演算として正しく定義される必要あり
// verified
//   - https://atcoder.jp/contests/arc042/submissions/75111096
//   - https://atcoder.jp/contests/abc186/submissions/75111166
//   - https://atcoder.jp/contests/abc222/submissions/75111823
//   - https://atcoder.jp/contests/abc270/submissions/75111892
//
// ============================================

#include <unordered_map>
#include <cmath>

namespace kwm_t::math {

template<
    class S,
    S(*op)(S, S),
    S(*inv)(S),
    S(*e)()
>
class BabyStepGiantStep {
private:
    long long sqrtp;
    std::unordered_map<S, long long> mp;

    S g;
    S invG;

public:

    /**
     * @brief コンストラクタ
     *
     * @param _g 基準元（g）
     * @param n  探索範囲の目安（mod や周期）
     */
    BabyStepGiantStep(S _g, long long n)
        : g(_g)
    {
        sqrtp = static_cast<long long>(std::sqrt(n)) + 1;
        build();
    }

private:

    /**
     * @brief baby-step の前計算
     */
    void build() {
        S cur = e();

        for (long long i = 0; i < sqrtp; i++) {
            if (!mp.count(cur)) {
                mp[cur] = i;
            }
            cur = op(cur, g);
        }

        invG = inv(cur);
    }

public:

    /**
     * @brief 離散対数を解く
     *
     * @param target g^x = target を解く
     * @return x（存在しない場合 -1）
     */
    long long solve(S target) {
        S cur = target;

        for (long long i = 0; i < sqrtp; i++) {
            auto it = mp.find(cur);
            if (it != mp.end()) {
                return i * sqrtp + it->second;
            }
            cur = op(cur, invG);
        }

        return -1;
    }
};

} // namespace kwm_t::math

#endif // KWM_T_MATH_BSGS_HPP