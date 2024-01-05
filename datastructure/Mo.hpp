#include <algorithm>
#include <vector>
#include <iostream>
#include <numeric>
struct Mo {
	int width;
	std::vector<int>left, right, order;
	std::vector<long long>ans;//štodoš
	//std::vector<long long>horder;
	int n, q;
	Mo(int _n, int _q) :n(_n), q(_q) {
		width = (int)sqrt(_n);
		order.resize(q);
		std::iota(order.begin(),order.end(), 0);
		ans.resize(q);
	}
	// add,query[l,r)
	void query(std::vector<int> &l, std::vector<int> &r) {
		left = l;
		right = r;
		//horder.resize(q);
		//rep(i, q) horder[i] = hilbertorder(left[i], right[i]);
	}
	// run
	void run(const std::vector<int>&data) {
		std::sort(order.begin(), order.end(), [&](int lh, int rh) {
			//return horder[lh] < horder[rh];
			int lblock = left[lh] / width;
			int rblock = left[rh] / width;
			if (lblock != rblock)return lblock < rblock;
			if (lblock & 1) return right[lh] < right[rh];
			return right[lh] > right[rh];
			});
		int nl = 0, nr = 0;
		// ‚±‚±‚©‚çštodoš
		std::vector<int>f(1000006, 0);
		int val = 0;
		auto add = [&](int idx) {
			if (f[data[idx]]++ == 0)val++;
		};
		auto del = [&](int idx) {
			if (f[data[idx]]-- == 1)--val;
		};
		auto ansset = [&](int idx) {
			ans[idx] = val;
		};
		//‚±‚±‚Ü‚Åštodoš
		for (auto idx : order) {
			while (nl > left[idx])add(--nl);
			while (nr < right[idx])add(nr++);
			while (nl < left[idx])del(nl++);
			while (nr > right[idx])del(--nr);
			ansset(idx);
		}
	}
	// TLE‚·‚é‚Æ‚«‚Í‚±‚Á‚¿‚ğg‚¤‚Æ’Ê‚é‚©‚à
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
			swap(x, y);
		}
		return d;
	}*/
};