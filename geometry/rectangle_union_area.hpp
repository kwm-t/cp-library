#ifndef RECTANGLE_UNION_AREA_HPP
#define RECTANGLE_UNION_AREA_HPP

#include <vector>
#include <algorithm>
#include <atcoder/lazysegtree>

namespace rectangle_union_area {

/**
 * @brief Area of Union of Rectangles
 *
 * 複数の軸平行長方形の union 面積を求める。
 * 各長方形は半開区間 [x1,x2) × [y1,y2) として扱う。
 *
 * Sweep Line + Lazy Segment Tree による典型実装。
 *
 * 典型用途:
 *   ・矩形 union 面積
 *   ・2D 典型問題 (AOJ / AtCoder / ICPC)
 *
 * 計算量:
 *   O(N log N)
 *
 * @tparam なし
 *
 * @param rects 長方形集合
 *
 * 制約 / 注意:
 *   ・rectangles が空なら 0 を返す
 *   ・座標は int を想定
 *   ・長方形は [x1,x2), [y1,y2) の半開区間
 *
 * 使用例:
 *
 *   vector<rectangle_union_area::Rect> rects;
 *   rects.push_back({0,3,0,2});
 *   rects.push_back({1,4,1,5});
 *
 *   long long area = rectangle_union_area::solve(rects);
 *
 * verified:
 *   AOJ DSL_4_A
 *   typical rectangle union problems
 *   https://judge.yosupo.jp/submission/359754
 */

struct Rect {
	int x1, x2, y1, y2;
	Rect() {}
	Rect(int x1, int x2, int y1, int y2)
		: x1(x1), x2(x2), y1(y1), y2(y2) {}
};

struct Event {
	int x;
	int y1, y2;
	int add;
	bool operator<(const Event& o) const { return x < o.x; }
};

namespace internal {

	struct S {
		long long min;
		long long cnt;
	};

	using F = long long;

	S op(S a, S b) {
		if (a.min < b.min) return a;
		if (a.min > b.min) return b;
		return { a.min, a.cnt + b.cnt };
	}

	S e() { return { (long long)4e18, 0 }; }

	S mapping(F f, S x) { return { x.min + f, x.cnt }; }

	F composition(F f, F g) { return f + g; }

	F id() { return 0; }

	using segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;

} // namespace internal


long long solve(const std::vector<Rect>& rects) {

	if (rects.empty()) return 0;

	std::vector<Event> ev;
	std::vector<int> ys;

	for (auto &r : rects) {
		ev.push_back({ r.x1, r.y1, r.y2, +1 });
		ev.push_back({ r.x2, r.y1, r.y2, -1 });
		ys.push_back(r.y1);
		ys.push_back(r.y2);
	}

	std::sort(ys.begin(), ys.end());
	ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

	auto get = [&](int y) {
		return std::lower_bound(ys.begin(), ys.end(), y) - ys.begin();
	};

	std::sort(ev.begin(), ev.end());

	std::vector<internal::S> init(ys.size() - 1);

	for (int i = 0; i + 1 < ys.size(); i++)
		init[i] = { 0, ys[i + 1] - ys[i] };

	internal::segtree seg(init);

	long long area = 0;
	int prev_x = ev[0].x;

	for (auto &e : ev) {

		auto s = seg.all_prod();

		if (s.min == 0) {
			long long uncovered = s.cnt;
			long long width = e.x - prev_x;
			area += (ys.back() - ys.front() - uncovered) * width;
		}
		else {
			area += (long long)(ys.back() - ys.front()) * (e.x - prev_x);
		}

		seg.apply(get(e.y1), get(e.y2), e.add);

		prev_x = e.x;
	}

	return area;
}

} // namespace rectangle_union_area

#endif // RECTANGLE_UNION_AREA_HPP