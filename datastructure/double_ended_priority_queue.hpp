
/**
 * @file double_ended_priority_queue.hpp
 * @brief 両端優先度付きキュー
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/data_structure/double_ended_priority_queue.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_DOUBLE_ENDED_PRIORITY_QUEUE_HPP
#define KWM_T_DATA_STRUCTURE_DOUBLE_ENDED_PRIORITY_QUEUE_HPP

#include <functional>
#include <iterator>
#include <set>
#include <utility>

namespace kwm_t::data_structure {

/**
 * @brief 両端から要素を取得・削除できる優先度付きキュー
 *
 * Compare による順序で要素を管理し、両端から要素を参照・削除できる。
 * 同じ要素の重複を許す。
 *
 * 典型用途:
 *  - 最小値と最大値の両方を頻繁に取得・削除する問題
 *  - 両端から要素を消費する貪欲法
 *  - スライディングウィンドウなどの両端管理
 *
 * 計算量:
 *  - push / emplace: O(log N)
 *  - front / back: O(1)
 *  - pop_front / pop_back: O(log N)
 *  - take_front / take_back: O(log N)
 *  - size / empty: O(1)
 *
 * @tparam T 要素の型
 * @tparam Container 内部コンテナ型。std::multiset<T, Compare> を指定する
 * @tparam Compare 要素の比較関数
 *
 * 制約 / 注意:
 *  - Container は std::multiset<T, Compare> として使用できる型であること
 *  - front / back / pop_front / pop_back / take_front / take_back は空でないこと
 *  - front は Compare における先頭要素を返す
 *  - back は Compare における末尾要素を返す
 *
 * 使用例:
 *  DoubleEndedPriorityQueue<int> pq;
 *  pq.push(3);
 *  pq.push(1);
 *  pq.push(5);
 *
 *  pq.front();      // 1
 *  pq.back();       // 5
 *  pq.take_front(); // 1 を取得して削除
 *  pq.pop_back();   // 5 を削除
 *
 * verified:
 *  https://atcoder.jp/contests/awc0205/submissions/80959509
 */
template<
    typename T,
    typename Container = std::multiset<T>,
    typename Compare = std::less<T>
>
class DoubleEndedPriorityQueue {
    Container st;

public:
    using value_type = T;
    using size_type = typename Container::size_type;

    /**
     * @brief 要素を追加する
     * @param x 追加する要素
     */
    void push(const T& x) {
        st.insert(x);
    }

    /**
     * @brief 要素をムーブして追加する
     * @param x 追加する要素
     */
    void push(T&& x) {
        st.insert(std::move(x));
    }

    /**
     * @brief 要素を直接構築して追加する
     * @tparam Args コンストラクタ引数の型
     * @param args コンストラクタ引数
     */
    template<typename... Args>
    void emplace(Args&&... args) {
        st.emplace(std::forward<Args>(args)...);
    }

    /**
     * @brief 先頭要素を参照する
     * @return 先頭要素へのconst参照
     */
    const T& front() const {
        return *st.begin();
    }

    /**
     * @brief 末尾要素を参照する
     * @return 末尾要素へのconst参照
     */
    const T& back() const {
        return *st.rbegin();
    }

    /**
     * @brief 先頭要素を削除する
     */
    void pop_front() {
        st.erase(st.begin());
    }

    /**
     * @brief 末尾要素を削除する
     */
    void pop_back() {
        st.erase(std::prev(st.end()));
    }

    /**
     * @brief 先頭要素を取得して削除する
     * @return 先頭要素
     */
    T take_front() {
        T res = front();
        pop_front();
        return res;
    }

    /**
     * @brief 末尾要素を取得して削除する
     * @return 末尾要素
     */
    T take_back() {
        T res = back();
        pop_back();
        return res;
    }

    /**
     * @brief 要素数を取得する
     * @return 要素数
     */
    size_type size() const {
        return st.size();
    }

    /**
     * @brief 空かどうかを判定する
     * @return 空ならtrue
     */
    bool empty() const {
        return st.empty();
    }

    /**
     * @brief 全要素を削除する
     */
    void clear() {
        st.clear();
    }
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_DOUBLE_ENDED_PRIORITY_QUEUE_HPP
