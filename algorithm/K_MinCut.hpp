template<class Cap>
class K_MinCut {
	int n;
	int source, sink;
	std::vector<int>index, sizes;
	std::vector<Cap>a;
	atcoder::mf_graph<Cap> graph;
	Cap R;
public:
	K_MinCut(std::vector<int>sizes) :n(sizes.size()), index(n), R(0) {
		int now = 0;
		for (int i = 0; i < n; ++i) {
			index[i] = now;
			now += sizes[i] - 1;
		}
		a.resize(now);
		source = now++;
		sink = now++;
		graph = atcoder::mf_graph<Cap>(now);
		this->sizes = move(sizes);
	}

	template<class F>
	void add_cost(int i, F f) {
		const int base = index[i];
		const int size = sizes[i];
		R += f(0);
		for (int d = 0; d < size - 1; ++d) {
			a[base + d] += f(d + 1) - f(d);
		}
	}
	template<class F>
	void add_cost(int i, int j, F f) {
		const int baseI = index[i];
		const int baseJ = index[j];
		const int sizeI = sizes[i];
		const int sizeJ = sizes[j];
		R += f(0, 0);
		for (int d = 0; d < sizeI - 1; d++) {
			a[baseI + d] += f(d + 1, 0) - f(d, 0);
		}
		for (int e = 0; e < sizeJ - 1; e++) {
			a[baseJ + e] += f(0, e + 1) - f(0, e);
		}
		for (int d = 0; d < sizeI - 1; d++) {
			for (int e = 0; e < sizeJ - 1; e++) {
				Cap cost = f(d, e) - f(d, e + 1) - f(d + 1, e) + f(d + 1, e + 1);
				assert(cost <= 0);
				if (!cost) continue;
				a[baseJ + e] += cost;
				graph.add_edge(baseI + d, baseJ + e, -cost);
			}
		}
	}

	Cap min_cut() {
		for (int i = 0; i < n; ++i) {
			const int base = index[i];
			const int size = sizes[i];
			for (int j = 0; j < size - 1; ++j) {
				if (0 == a[base + j])continue;
				if (a[base + j] > 0) {
					graph.add_edge(source, base + j, a[base + j]);
				}
				else {
					R += a[base + j];
					graph.add_edge(base + j, sink, -a[base + j]);
				}
			}
			// tttt....ssssを強制させる
			for (int j = 0; j < size - 2; ++j) {
				graph.add_edge(base + j, base + j + 1, std::numeric_limits<Cap>::max());
			}
		}
		return R + graph.flow(source, sink);
	}
};
// https://kanpurin.hatenablog.com/entry/2022/08/05/065209
// https://mojacoder.app/users/_kanpurin_/problems/project_selection_problem003