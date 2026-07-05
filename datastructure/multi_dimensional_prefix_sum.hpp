#ifndef KWM_T_DATA_STRUCTURE_MULTI_DIMENSIONAL_PREFIX_SUM_HPP
#define KWM_T_DATA_STRUCTURE_MULTI_DIMENSIONAL_PREFIX_SUM_HPP

#include <cassert>
#include <vector>

namespace kwm_t::data_structure {

    /**
     * @brief 任意次元累積和
     *
     * build() を呼ぶことで d 次元累積和を構築する。
     * query() は半開区間 [l,r) を返す。
     *
     * 計算量:
     *  構築 : O(D * ΠNi)
     *  クエリ : O(2^D)
     *
     * @tparam T 加算可能な型
     *
     * 制約:
     *  - build() 後は値を書き換えないこと
     *  - query() は半開区間
     *
     * 使用例:
     * @code
     * MultiDimensionalPrefixSum<int> ps(3,4,5);
     *
     * ps.at(1,2,3) += 10;
     * ps.at(2,0,1) += 5;
     *
     * ps.build();
     * // [1,3) × [0,4) × [1,5)
     * int ans = ps.query(
     *     1,3,
     *     0,4,
     *     1,5
     * );
     * @endcode
     *
     * verified:
     *  https://atcoder.jp/contests/abc465/submissions/77230742
     */
    template <class T>
    class MultiDimensionalPrefixSum
    {
    public:
        explicit MultiDimensionalPrefixSum(const std::vector<int> &sizes)
            : dim_(static_cast<int>(sizes.size())),
              size_(sizes)
        {

            for (int &x : size_)
            {
                ++x;
            }

            stride_.resize(dim_ + 1);
            stride_[0] = 1;

            for (int i = 0; i < dim_; i++)
            {
                stride_[i + 1] = stride_[i] * size_[i];
            }

            data_.assign(stride_[dim_], T());
        }

        template <class... Args>
        explicit MultiDimensionalPrefixSum(Args... sizes)
            : MultiDimensionalPrefixSum(
                  std::vector<int>{static_cast<int>(sizes)...}) {}

        template <class... Args>
        T &at(Args... index)
        {
            return data_[to_index(index...)];
        }

        T &at(const std::vector<int> &index)
        {
            return data_[to_index(index)];
        }

        void build()
        {
            const int n = stride_[dim_];

            for (int d = 0; d < dim_; d++)
            {
                for (int i = 0; i < n; i++)
                {
                    int j = i + stride_[d];

                    if (j / stride_[d + 1] == i / stride_[d + 1])
                    {
                        data_[j] += data_[i];
                    }
                }
            }
        }

        template <class... Args>
        T query(Args... args) const
        {
            static_assert(sizeof...(Args) % 2 == 0);

            return query(std::vector<int>{static_cast<int>(args)...});
        }

        T query(const std::vector<int> &range) const
        {
            assert(static_cast<int>(range.size()) == dim_ * 2);

            for (int d = 0; d < dim_; d++)
            {
                if (range[2 * d] >= range[2 * d + 1])
                {
                    return T();
                }
            }

            T result = T();

            for (int bit = 0; bit < (1 << dim_); bit++)
            {
                int id = 0;
                int sign = 1;

                for (int d = 0; d < dim_; d++)
                {
                    int x;

                    if ((bit >> d) & 1)
                    {
                        x = range[2 * d];
                        sign = -sign;
                    }
                    else
                    {
                        x = range[2 * d + 1];
                    }

                    assert(0 <= x && x < size_[d]);

                    id += x * stride_[d];
                }

                result += data_[id] * sign;
            }

            return result;
        }

    private:
        int dim_;
        std::vector<int> size_;
        std::vector<int> stride_;
        std::vector<T> data_;

        int to_index(const std::vector<int> &index) const
        {
            assert(static_cast<int>(index.size()) == dim_);

            int id = 0;

            for (int i = 0; i < dim_; i++)
            {
                assert(0 <= index[i] && index[i] + 1 < size_[i]);

                id += (index[i] + 1) * stride_[i];
            }

            return id;
        }

        template <class... Args>
        int to_index(Args... args) const
        {
            static_assert(sizeof...(Args) >= 1);

            assert(static_cast<int>(sizeof...(Args)) == dim_);

            return to_index(std::vector<int>{static_cast<int>(args)...});
        }
};

} // namespace kwm_t::data_structure

#endif