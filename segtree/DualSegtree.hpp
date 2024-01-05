#include <algorithm>
#include <cassert>
#include <vector>
const int INF = 1e9;
const long long LINF = 1e18;
template<class S, S op(S l, S r), S e()>
struct DualSegtree {
private:
	int mn = 0;
	int mN = 0;
	int mlogN = 0;
	std::vector<S> A;
	void propagate(int i) {
		if (i >= mN) return;
		A[i * 2] = op(A[i * 2], A[i]);
		A[i * 2 + 1] = op(A[i * 2 + 1], A[i]);
		A[i] = e();
	}
	void full_propagate(int i) {
		for (int d = mlogN; d > 0; d--) propagate(i >> d);
	}
public:
	DualSegtree(int n = 0) {
		mn = n;
		mlogN = 0; mN = 1;
		while (mN < n) { mN *= 2; mlogN++; }
		A.assign(mN * 2, e());
	}
	void point_init(int p) {
		assert(0 <= p); assert(p < mn);
		p += mN;
		full_propagate(p);
		A[p] = e();
	}
	S get(int p) {
		assert(0 <= p); assert(p < mn);
		p += mN;
		full_propagate(p);
		return A[p];
	}
	void apply(int l, int r, S x) {
		l += mN;
		r += mN;
		if (l != mN) full_propagate(l - 1);
		if (r != mN * 2) full_propagate(r);
		while (l < r) {
			if (l & 1) { A[l] = op(A[l], x); l++; }
			if (r & 1) { r--; A[r] = op(A[r], x); }
			l /= 2; r /= 2;
		}
	}
};
//区間加算、一点取得
using S0 = long long;
S0 op0(S0 x, S0 f) { return { x + f }; }
S0 e0() { return { 0 }; }
//区間更新、一点取得
using S1 = long long;
S1 op1(S1 x, S1 f) { return (f == -LINF ? x : f); }
S1 e1() { return { -LINF }; }
//区間chmin、一点取得
using S = long long;
S op0(S x, S f) { return std::min(x, f); }
S e0() { return { INF }; }
//DualSegtree<S, op, e>seg(n);