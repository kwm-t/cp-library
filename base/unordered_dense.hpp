/**
 * @file unordered_dense.hpp
 * @brief 高速な unordered コンテナ
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/base/unordered_dense.hpp
 */
#ifndef KWM_T_BASE_UNORDERED_DENSE_HPP
#define KWM_T_BASE_UNORDERED_DENSE_HPP

/**
 * @brief ローカル環境用(MSC)hashmap/hashset
 */
#ifdef ATCODER
#include <ankerl/unordered_dense.h>
template <typename K, typename V>
using hash_map = ankerl::unordered_dense::map<K, V>;
template <typename K>
using hash_set = ankerl::unordered_dense::set<K>;
#else
#include <unordered_map>
#include <unordered_set>
template <typename K, typename V>
using hash_map = std::unordered_map<K, V>;
template <typename K>
using hash_set = std::unordered_set<K>;
#endif

#endif // KWM_T_BASE_UNORDERED_DENSE_HPP