#ifndef KWM_T_DATA_STRUCTURE_ERASABLE_PRIORITY_QUEUE_HPP
#define KWM_T_DATA_STRUCTURE_ERASABLE_PRIORITY_QUEUE_HPP

#include <queue>
#include <cstddef>

namespace kwm_t::data_structure {

/**
 * @brief Erasable Priority Queue
 *
 * 典型用途:
 *   - priority_queue からの遅延削除
 *   - ダイクストラ・イベント管理・動的集合
 *
 * 計算量:
 *   - add: O(log N)
 *   - erase: O(log N)
 *   - top: amortized O(log N)
 *
 * 注意:
 *   - erase は「存在することが保証される値」を想定
 */
template <typename T>
class ErasablePriorityQueue {
public:
    ErasablePriorityQueue() = default;

    void add(const T &val) {
        pq.push(val);
    }

    void erase(const T &val) {
        dpq.push(val);
    }

    T top(const T &empty) {
        while (!dpq.empty() && pq.top() == dpq.top()) {
            pq.pop();
            dpq.pop();
        }
        if (pq.empty()) return empty;
        return pq.top();
    }

    std::size_t size() {
        return pq.size() - dpq.size();
    }

    bool empty() {
        return size() == 0;
    }

private:
    std::priority_queue<T> pq, dpq;
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_ERASABLE_PRIORITY_QUEUE_HPP