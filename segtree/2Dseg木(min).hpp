#include<vector>
#include <algorithm>
const long long LINF = 1e18;

struct segtree2D {
	int H, W;
	std::vector<std::vector<long long>> dat;
	segtree2D() {}
	segtree2D(std::vector<std::vector<long long>> &f) {
		H = W = 1;
		while (H < (int)f.size()) H <<= 1;
		while (W < (int)f[0].size()) W <<= 1;
		dat.assign(2 * H - 1, std::vector<long long>(2 * W - 1, LINF));
		init(f);
	}
	void init(std::vector<std::vector<long long>> &f) {
		for (int i = 0; i < (int)f.size(); i++)
			for (int j = 0; j < (int)f[0].size(); j++)
				dat[i + H - 1][j + W - 1] = f[i][j];
		for (int i = 2 * H - 2; i > H - 2; i--)
			for (int j = W - 2; j >= 0; j--)
				dat[i][j] = std::min(dat[i][2 * j + 1], dat[i][2 * j + 2]);
		for (int i = H - 2; i >= 0; i--)
			for (int j = 0; j < 2 * W - 1; j++)
				dat[i][j] = std::min(dat[2 * i + 1][j], dat[2 * i + 2][j]);
	}
	long long minimum(int li, int lj, int ri, int rj) { return minimum_h(li, lj, ri, rj, 0, H, 0); }
	long long minimum_h(int li, int lj, int ri, int rj, int si, int ti, int k) {
		if (ri <= si or ti <= li) return LINF;
		if (li <= si and ti <= ri) return minimum_w(lj, rj, 0, W, k, 0);
		const int mi = (si + ti) / 2;
		return std::min(minimum_h(li, lj, ri, rj, si, mi, 2 * k + 1), minimum_h(li, lj, ri, rj, mi, ti, 2 * k + 2));
	}
	long long minimum_w(int lj, int rj, int sj, int tj, int i, int k) {
		if (rj <= sj or tj <= lj) return -LINF;
		if (lj <= sj and tj <= rj) return dat[i][k];
		const int mj = (sj + tj) / 2;
		return std::min(minimum_w(lj, rj, sj, mj, i, 2 * k + 1), minimum_w(lj, rj, mj, tj, i, 2 * k + 2));
	}
};