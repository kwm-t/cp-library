/*struct dsuWrapper : public atcoder::dsu {
public:
	dsuWrapper(int n) :atcoder::dsu(n) {}

	int merge(int a, int b, const std::function<void(int, int, int)> &merge_data) {
		if (atcoder::dsu::same(a, b)) {
			return atcoder::dsu::leader(a);
		}
		int la = atcoder::dsu::leader(a);
		int lb = atcoder::dsu::leader(b);
		int l = atcoder::dsu::merge(a, b);
		merge_data(la, lb, l);
		return l;
	}
};*/

template<typename Data, Data(*data_merge)(Data&, Data&)>
struct dsuWrapper : public atcoder::dsu {
public:
	dsuWrapper(int n, std::vector<Data>data) :atcoder::dsu(n), data(data) {}

	int merge(int a, int b) {
		if (atcoder::dsu::same(a, b)) {
			return atcoder::dsu::leader(a);
		}
		a = atcoder::dsu::leader(a);
		b = atcoder::dsu::leader(b);
		int l = atcoder::dsu::merge(a, b);
		data[l] = data_merge(data[a], data[b]);
		return l;
	}
	Data get_value(int a) {
		return data[atcoder::dsu::leader(a)];
	}
private:
	std::vector<Data>data;
};
// ex.
// int merge(int &a, int &b) {
//	 std::swap(a, b);
//	 return a + b;
// }
// dsuWrapper<int, merge> dsu(n, d);