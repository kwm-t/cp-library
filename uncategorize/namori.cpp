	vector<int>roop;
	set<int>st;
	auto dfs = [&](auto self, int v, int p = -1)->bool {
		if (st.count(v)) {
			reverse(all(roop));
			while (v != roop.back()) roop.pop_back();
			reverse(all(roop));
			return true;
		}
		st.insert(v);
		roop.push_back(v);
		for (auto nv : to[v]) {
			if (p == nv)continue;
			auto get = self(self, nv, v);
			if (get)return true;
		}
		roop.pop_back();
		st.erase(v);
		return false;
	};