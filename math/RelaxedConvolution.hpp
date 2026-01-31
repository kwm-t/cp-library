// RelaxedConvolution
// O(N(logN)^2)
// 配列A,配列Bの各項が前から与えられたときに
// 都度畳み込み結果Cの各項を返す
// https://qiita.com/Kiri8128/items/1738d5403764a0e26b4c
class RelaxedConvolution {
public:
	RelaxedConvolution(int n) : a(n), b(n), c(n), k(0) {}
	mint operator()(mint na, mint nb) {
		a[k] = na;
		b[k] = nb;
		int s = k + 2;
		for (int w = 1; s % w == 0 && w < s; w <<= 1) {
			// w*wの領域をFFTする
			for (int t = 0; t < 2; ++t) {
				// ダブルカウントを防ぐ
				if (t == 0 || w * 2 != s) {
					std::vector<mint>f(w), g(w);
					for (int i = 0; i < w; ++i) {
						// [w - 1,w * 2 - 1) *  [k - w + 1,k + 1)
						f[i] = a[w - 1 + i];
						g[i] = b[k - w + 1 + i];
					}
					auto h = atcoder::convolution(f, g);
					for (int i = 0, j = k; i < h.size() && j < c.size(); i++, j++) c[j] += h[i];
				}
				swap(a, b);
			}
		}
		return c[k++];
	}
private:
	std::vector<mint>a, b, c;
	int k = 0;
};