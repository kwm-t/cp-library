vector<int> g(string s, string t, int m) {
	vector<int>ret((s.size() + t.size()));
	reverse(all(s));
	reverse(all(t));
	rep(i, s.size()) {
		rep(j, t.size()) {
			int x = s[i] - '0';
			int y = t[j] - '0';
			int z = x * y;
			int idx = i + j;
			while (z) {
				ret[idx] += z % m;
				z /= m;
				idx++;
			}
		}
	}
	rep(i, ret.size() - 1) {
		ret[i + 1] += ret[i] / m;
		ret[i] %= m;
	}
	while (ret.size() >= 2 && ret.back() == 0) ret.pop_back();
	reverse(all(ret));
	return ret;
}