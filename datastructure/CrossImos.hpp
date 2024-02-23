template <class T>
class CrossImos {
	// 斜めimos
public:
	CrossImos(int h, int w) :h(h), w(w) {
		init();
	}
	void init() {
		imos0.assign(h + 1, std::vector<T>(w + 1));
		imos1.assign(h + 1, std::vector<T>(w + 1));
	}
	// 0:左上->1;右下(引数は閉区間)
	void add0(int i0, int j0, int i1, int j1, T val) {
		imos0[i0 + 0][j0 + 0] += val;
		imos0[i1 + 1][j1 + 1] -= val;
	}
	// 0:右上→1;左下(引数は閉区間)
	void add1(int i0, int j0, int i1, int j1, T val) {
		imos1[i0 + 0][j0 + 1] += val;
		imos1[i1 + 1][j1 + 0] -= val;
	}
	T get0(int i, int j) {
		return imos0[i][j];
	}
	T get1(int i, int j) {
		return imos1[i][j + 1];
	}
	void build() {
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				imos0[i + 1][j + 1] += imos0[i + 0][j + 0];
				imos1[i + 1][j + 0] += imos1[i + 0][j + 1];
			}
		}
	}
	void dump() {
		for (int i = 0; i < h + 1; ++i) {
			for (int j = 0; j < w + 1; ++j) {
				std::cout << imos0[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < h + 1; ++i) {
			for (int j = 0; j < w + 1; ++j) {
				std::cout << imos1[i][j] << " ";
			}
			std::cout << std::endl;
		}
	};
private:
	int h, w;
	std::vector<std::vector<T>>imos0;//左上→右下
	std::vector<std::vector<T>>imos1;//右上→左下
};