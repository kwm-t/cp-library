template <class T>
class CrossPrefixSum {
	// 斜め累積和
public:
	CrossPrefixSum(int h, int w, const std::vector<std::vector<T>> &org) :h(h), w(w) {
		init(org);
	}
	// 0:左上->1;右下(引数は閉区間)
	T getRui0(int i0, int j0, int i1, int j1) {
		return rui0[i1 + 1][j1 + 1] - rui0[i0][j0];
	}
	// 0:右上→1;左下(引数は閉区間)
	T getRui1(int i0, int j0, int i1, int j1) {
		return rui1[i1 + 1][j1] - rui1[i0][j0 + 1];
	};
private:
	int h, w;
	std::vector<std::vector<T>>rui0;//左上→右下
	std::vector<std::vector<T>>rui1;//右上→左下
	void init(const std::vector<std::vector<T>> &org) {
		rui0.assign(h + 1, std::vector<T>(w + 1));
		rui1.assign(h + 1, std::vector<T>(w + 1));
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				rui0[i + 1][j + 1] = org[i][j];
				rui1[i + 1][j + 0] = org[i][j];
			}
		}
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				rui0[i + 1][j + 1] += rui0[i + 0][j + 0];
				rui1[i + 1][j + 0] += rui1[i + 0][j + 1];
			}
		}
	}
};