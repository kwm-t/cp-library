template <typename T>
class RangeManeger {
private:
	std::set<std::tuple<int, int, T>> st;
	int size;
	void init(const std::vector<T>& data) {
		size = (int)data.size();
		for (int i = 0; i < size;) {
			int j = i + 1;
			while (j < size && data[i] == data[j])j++;
			st.emplace(i, j, data[i]);
			i = j;
		}
	}

public:
	RangeManeger(const std::vector<T>& data) {
		init(data);
	}
	std::vector<std::tuple<int, int, T>> update(int l, int r, const T val) {
		auto ite = std::prev(st.lower_bound({ l + 1, l + 1, T(0) }));
		std::vector<std::tuple<int, int, T>> add, res;
		int nl = l, nr = r;
		while (ite != st.end() && std::get<0>(*ite) < r) {
			auto[_l, _r, _val] = *ite;
			if (_l < l) {
				if (val == _val) nl = _l;
				else add.emplace_back(_l, l, _val);
			}
			if (r < _r) {
				if (val == _val) nr = _r;
				else add.emplace_back(r, _r, _val);
			}
			res.emplace_back(std::max(l, _l), std::min(r, _r), _val);
			ite = st.erase(ite);
		}

		ite = st.lower_bound({ nl, nl, T(0) });
		if (ite != st.begin()) {
			ite = std::prev(ite);
			auto[_l, _r, _val] = *ite;
			if (val == _val) {
				nl = _l;
				st.erase(ite);
			}
		}

		ite = st.lower_bound({ nr + 1, nr + 1, T(0) });
		if (ite != st.begin()) {
			ite = std::prev(ite);
			auto[_l, _r, _val] = *ite;
			if (val == _val) {
				nr = _r;
				st.erase(ite);
			}
		}
		add.emplace_back(nl, nr, val);
		for (auto[l, r, val] : add)
			st.emplace(l, r, val);
		return res;
	}
	std::tuple<int, int, T> get(int index) {
		auto ite = std::prev(st.lower_bound({ index + 1, index + 1, T(0) }));
		return *ite;
	}
	void debug() {
		std::cout << "debug::start" << std::endl;
		for (auto[l, r, val] : st) {
			std::cout << l << " " << r << " " << val << std::endl;
		}
		std::cout << "debug::end" << std::endl;
	}
};