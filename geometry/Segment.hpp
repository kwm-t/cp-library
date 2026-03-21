#ifndef KWM_T_GEOMETRY_SEGMENT_HPP
#define KWM_T_GEOMETRY_SEGMENT_HPP

#include "point.hpp"

/**
 * @brief Line Segment
 *
 * 線分ライブラリ
 *
 * 典型用途:
 *   ・線分交差判定
 *   ・点が線分上にあるか
 *   ・平行判定
 *
 * 計算量:
 *   O(1)
 *
 * @tparam T 座標型
 *
 * verified:
 *   AOJ CGL 系
 */

namespace kwm_t::geometry {

	template<class T>
	struct Segment {

		Point<T> a, b;

		Segment() {}
		Segment(Point<T> a, Point<T> b) : a(a), b(b) {}

		/**
		 * @brief 点が線分上にあるか
		 */
		bool contains(const Point<T>& p) const {

			if (p == a || p == b) return true;

			Point<T> v0 = a - p;
			Point<T> v1 = b - p;

			if (v0.cross(v1) != 0) return false;

			return v0.dot(v1) < 0;
		}

		/**
		 * @brief 平行判定
		 */
		bool parallel(const Segment& s) const {
			return (b - a).cross(s.b - s.a) == 0;
		}

		/**
		 * @brief 線分交差判定（端点含む）
		 */
		bool intersect(const Segment& s) const {

			if (parallel(s)) {
				if (contains(s.a)) return true;
				if (contains(s.b)) return true;
				if (s.contains(a)) return true;
				if (s.contains(b)) return true;
				return false;
			}

			auto v0 = b - a;
			auto v1 = s.a - a;
			auto v2 = s.b - a;

			if (v0.cross(v1) * v0.cross(v2) > 0) return false;

			v0 = s.b - s.a;
			v1 = a - s.a;
			v2 = b - s.a;

			if (v0.cross(v1) * v0.cross(v2) > 0) return false;

			return true;
		}

	};

}

#endif // KWM_T_GEOMETRY_SEGMENT_HPP