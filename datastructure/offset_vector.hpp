#ifndef KWM_T_DATASTRUCTURE_OFFSET_VECTOR_HPP
#define KWM_T_DATASTRUCTURE_OFFSET_VECTOR_HPP

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

namespace kwm_t::datastructure {

/**
 * @brief 負の添字を扱える疎配列
 *
 * 添字範囲は半開区間 [l, r)
 *
 * std::map<int,T> や std::unordered_map<int,T> の代替を想定。
 * T{} を未使用状態として扱う。
 *
 * clear() は触った要素のみ初期化するため
 * O(r - l) ではなく O(操作回数)。
 *
 * 計算量:
 *   operator[] : O(1)
 *   contains   : O(1)
 *   erase      : O(1)
 *   clear      : O(触った要素数)
 *   foreach    : O(触った要素数)
 *
 * @tparam T 値型
 *
 * 制約 / 注意:
 *   T{} を未使用状態として扱う。
 *   そのため T{} が有効値となる用途には向かない。
 *
 * 使用例:
 *
 *   OffsetVector<long long> dp(-200000, 200001);
 *
 *   dp[0] = 1;
 *   dp[-5] += 3;
 *
 *   if (dp.contains(-5)) {
 *       ...
 *   }
 *
 *   dp.clear();
 *
 * verified:
 *   https://atcoder.jp/contests/awc0088/submissions/76567559
 */
template<class T>
class OffsetVector {
private:
    int l_;
    int r_;

    std::vector<T> data_;

    std::vector<int> touched_;
    std::vector<char> touched_flag_;

    int index(int key) const {
        assert(l_ <= key && key < r_);
        return key - l_;
    }

    void touch(int idx) {
        if (!touched_flag_[idx]) {
            touched_flag_[idx] = 1;
            touched_.push_back(idx);
        }
    }

public:
    OffsetVector() = default;

    OffsetVector(int l, int r)
        : l_(l),
          r_(r),
          data_(r - l, T{}),
          touched_flag_(r - l, 0) {
        assert(l <= r);
    }

    T& operator[](int key) {
        int idx = index(key);
        touch(idx);
        return data_[idx];
    }

    const T& operator[](int key) const {
        return data_[index(key)];
    }

    bool contains(int key) const {
        return data_[index(key)] != T{};
    }

    void erase(int key) {
        int idx = index(key);
        touch(idx);
        data_[idx] = T{};
    }

    void set(int key, const T& value) {
        int idx = index(key);
        touch(idx);
        data_[idx] = value;
    }

    void add(int key, const T& value) {
        int idx = index(key);
        touch(idx);
        data_[idx] += value;
    }

    void clear() {
        for (int idx : touched_) {
            data_[idx] = T{};
            touched_flag_[idx] = 0;
        }
        touched_.clear();
    }

    int l() const {
        return l_;
    }

    int r() const {
        return r_;
    }

    int size() const {
        int res = 0;

        for (int idx : touched_) {
            if (data_[idx] != T{}) {
                ++res;
            }
        }

        return res;
    }

    template<class F>
    void foreach(F f) const {
        for (int idx : touched_) {
            if (data_[idx] != T{}) {
                f(idx + l_, data_[idx]);
            }
        }
    }

    std::vector<std::pair<int, T>> items() const {
        std::vector<std::pair<int, T>> res;

        for (int idx : touched_) {
            if (data_[idx] != T{}) {
                res.emplace_back(idx + l_, data_[idx]);
            }
        }

        return res;
    }

    void dump(std::ostream& os = std::cerr) const {
        os << "{";

        bool first = true;

        for (int idx : touched_) {
            if (data_[idx] == T{}) {
                continue;
            }

            if (!first) {
                os << ", ";
            }

            os << (idx + l_) << ":" << data_[idx];

            first = false;
        }

        os << "}\n";
    }
};

} // namespace kwm_t::datastructure

#endif // KWM_T_DATASTRUCTURE_OFFSET_VECTOR_HPP