template<class S, S op(S l, S r), S e()>
struct SegmentTree2D {
public:
	SegmentTree2D(int h, int w) {
		init(h, w);
	}
	SegmentTree2D(const std::vector<std::vector<S>> &a) {
		int h = (int)a.size();
		int w = (int)a[0].size();
		init(h, w);
		allSet(h, w, a);
	}
	// buildより前に呼ぶ
	void set(const int &h, const int &w, const S &x) {
		seg[index(h + H, w + W)] = x;
	}
	// !!!必須!!!
	void build() {
		for (int w = W; w < 2 * W; w++) {
			for (int h = H - 1; h; h--) {
				seg[index(h, w)] = op(seg[index(2 * h + 0, w)], seg[index(2 * h + 1, w)]);
			}
		}
		for (int h = 0; h < 2 * H; h++) {
			for (int w = W - 1; w; w--) {
				seg[index(h, w)] = op(seg[index(h, 2 * w + 0)], seg[index(h, 2 * w + 1)]);
			}
		}
	}
	S get(const int &h, const int &w) {
		return seg[index(h + H, w + W)];
	}
	void update(int h, int w, const S &x) {
		seg[index(h + H, w + W)] = x;
		h += H, w += W;
		for (int i = h >> 1; i; i >>= 1) {
			seg[index(i, w)] = op(seg[index(2 * i + 0, w)], seg[index(2 * i + 1, w)]);
		}
		for (; h; h >>= 1) {
			for (int j = w >> 1; j; j >>= 1) {
				seg[index(h, j)] = op(seg[index(h, 2 * j + 0)], seg[index(h, 2 * j + 1)]);
			}
		}
	}
	// [(h1,w1),(h2,w2))
	S query(int h1, int w1, int h2, int w2) {
		if (h1 >= h2 || w1 >= w2)return e();
		S ret = e();
		h1 += H, w1 += W, h2 += H, w2 += W;
		for (; h1 < h2; h1 >>= 1, h2 >>= 1) {
			if (h1 & 1) ret = op(ret, _inner_query(h1, w1, w2)), h1++;
			if (h2 & 1) --h2, ret = op(ret, _inner_query(h2, w1, w2));
		}
		return ret;
	}
private:
	int H, W;
	std::vector<S> seg;

	void init(int h, int w) {
		H = 1, W = 1;
		while (H < h)H <<= 1;
		while (W < w)W <<= 1;
		seg.assign(4 * H * W, e());
	}
	void allSet(int h, int w, const std::vector<std::vector<S>> &a) {
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				set(i, j, a[i][j]);
			}
		}
	}
	int index(int h, int w) {
		return h * 2 * W + w;
	}
	S _inner_query(int h, int w1, int w2) {
		S res = e();
		for (; w1 < w2; w1 >>= 1, w2 >>= 1) {
			if (w1 & 1) res = op(res, seg[index(h, w1)]), w1++;
			if (w2 & 1) --w2, res = op(res, seg[index(h, w2)]);
		}
		return res;
	}
};
// build()の呼び出し必須
// ex.
// using S = long long;
// S op(S x, S y) { return std::max(x, y); }
// S e() { return { -LINF }; }
// SegmentTree2D<S, op, e>seg(a);