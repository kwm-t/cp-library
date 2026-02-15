int val(char c) {
	if ('0' <= c && c <= '9') return c - '0';
	if ('A' <= c && c <= 'Z') return c - 'A' + 10;
	if ('a' <= c && c <= 'z') return c - 'a' + 10;
	return -1;
}

char dig(int x) {
	if (x < 10) return char('0' + x);
	return char('A' + (x - 10));
}

// p進数文字列 s を q で割る（商と余りを返す）
std::string div_base(const std::string& s, int p, int q, int &rem) {
	std::vector<int> a;
	for (char c : s) a.push_back(val(c));

	std::vector<int> nxt;
	long long carry = 0;
	for (int x : a) {
		carry = carry * p + x;
		int d = carry / q;
		carry %= q;
		if (!nxt.empty() || d != 0) nxt.push_back(d);
	}
	rem = carry;

	if (nxt.empty()) return "0";

	std::string t;
	for (int x : nxt) t.push_back(dig(x));
	return t;
}

std::string convert_base(std::string s, int p, int q) {
	if (s == "0") return "0";
	if (p == q)return s;

	std::string res;
	while (!(s.size() == 1 && s[0] == '0')) {
		int rem;
		s = div_base(s, p, q, rem);
		res.push_back(dig(rem));
	}
	std::reverse(res.begin(), res.end());
	return res;
}