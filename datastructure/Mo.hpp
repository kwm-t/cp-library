#include <algorithm>
#include <vector>
#include <iostream>
#include <numeric>
#include <functional>
struct Mo {
	int width;
	std::vector<int>left, right, order;
	std::vector<int>ans;
	//std::vector<long long>horder;
	int n, q;
	Mo(int _n, int _q) :n(_n), q(_q) {
		width = (int)sqrt(_n);
		order.resize(q);
		std::iota(order.begin(), order.end(), 0);
		ans.resize(q);
	}
	// add query[l,r)
	void query(std::vector<int> &l, std::vector<int> &r) {
		left = l;
		right = r;
		/*
		horder.resize(q);
		for (int i = 0; i < q; ++i) horder[i] = hilbertorder(left[i], right[i]);
		*/
	}
	// run
	void run(const std::function<void(int, int, int)> &upd, const std::function<void(int)> &set) {
		std::sort(order.begin(), order.end(), [&](int lh, int rh) {
			//return horder[lh] < horder[rh];
			int lblock = left[lh] / width;
			int rblock = left[rh] / width;
			if (lblock != rblock)return lblock < rblock;
			if (lblock & 1) return right[lh] < right[rh];
			return right[lh] > right[rh];
			});
		int nl = 0, nr = 0;
		for (auto idx : order) {
			while (nl > left[idx])upd(0, nl--, nr);
			while (nr < right[idx])upd(1, nl, ++nr);
			while (nl < left[idx])upd(2, ++nl, nr);
			while (nr > right[idx])upd(3, nl, nr--);
			set(idx);
		}
	}
	// TLE‚·‚é‚Æ‚«‚Í‚±‚Á‚¿‚ðŽg‚¤‚Æ’Ê‚é‚©‚à
	/*const int logn = 20;
	const int maxn = 1 << logn;
	long long hilbertorder(int x, int y) {
		long long d = 0;
		for (int s = 1 << logn - 1; s; s >>= 1) {
			bool rx = x & s, ry = y & s;
			d = d << 2 | rx * 3 ^ static_cast<int>(ry);
			if (ry) continue;
			if (rx) {
				x = maxn - x;
				y = maxn - y;
			}
			std::swap(x, y);
		}
		return d;
	}*/
};
/*
	Example
	https://atcoder.jp/contests/abc384/submissions/60782036
	Mo mo(n, q);
	auto upd = [&](int type, int l, int r) {
		if (type == 0) {// l--
			// TODO
		}
		else if (type == 1) {// r++
			// TODO
		}
		else if (type == 2) {// l++
			// TODO
		}
		else if (type == 3) {// r--
			// TODO
		}
	};
	auto set = [&](int idx) {
		// ans[idx] = val;
	};
	mo.run(upd, set);
*/