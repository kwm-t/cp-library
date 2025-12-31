class SuffixArray {
public:
	SuffixArray(std::string s) :S(s) {
		build();
	}
	std::vector<int> operator()() {
		return SA;
	}
private:
	std::string S;
	std::vector<int>SA;
	const char SENTINEL = '$';
	const int KIND = 128;
	void build() {
		// 番兵追加
		S += SENTINEL;

		// char->int
		std::vector<int>str(S.size());
		for (int i = 0; i < (int)S.size(); ++i) str[i] = S[i] - SENTINEL;

		// sa-isでSuffixArrayを求める
		SA = sa_is(str, KIND);

		// 番兵を取り除く
		SA.erase(SA.begin());
	}

	// sa-isでSuffixArrayを求める
	std::vector<int> sa_is(const std::vector<int> &str, const int &kind) {
		const int n = str.size();
		// Step0
		// S/L,LMSを前計算しておく
		auto is_s = calcS(str);
		auto is_lms = calcLMS(is_s);
		std::vector<int>lms_pos;
		for (int i = 0; i < n; ++i)if (is_lms[i])lms_pos.push_back(i);

		// Step1
		// induced sortを行い疑似SAを求める
		// LMS部分文字列の辞書順を取得する
		auto pseudo_sa = induced_sort(str, lms_pos, is_s, kind);

		// Step2
		// LMS の suffixの辞書順を求める
		std::vector<int> lms_order;
		lms_order.reserve((int)lms_pos.size());
		for (auto e : pseudo_sa)if (is_lms[e]) lms_order.push_back(e);

		// 連続するLMS部分文字列の比較を行う
		int rank = 0;
		pseudo_sa[lms_order[0]] = rank;
		if ((int)lms_order.size() > 1) pseudo_sa[lms_order[1]] = ++rank;
		for (int i = 1; i < (int)lms_order.size() - 1; ++i) {
			bool diff = false;
			for (int j = 0; j < n; ++j) {
				int idx0 = lms_order[i + 0] + j;
				int idx1 = lms_order[i + 1] + j;
				if (str[idx0] != str[idx1] || is_lms[idx0] != is_lms[idx1]) {
					diff = true;
					break;
				}
				if (0 != j && is_lms[idx0])break;
			}
			pseudo_sa[lms_order[i + 1]] = diff ? ++rank : rank;
		}

		// LMS部分文字列を一つのcharとしてみなしたn_Strを作る
		std::vector<int> n_str;
		n_str.reserve((int)lms_pos.size());
		int index = 0;
		for (int i = 0; i < n; ++i) if (is_lms[i]) {
			n_str.push_back(pseudo_sa[i]);
		}

		// sa_isを再帰的に行いLMSのSuffixArrayを求める
		std::vector<int>lms_sa;
		if (rank + 1 == lms_pos.size()) {
			// LMS部分文字列に重複がなければ辞書順が正しく求まっている
			lms_sa = lms_order;
		}
		else {
			// 再帰的にSA-IS法を適当
			lms_sa = sa_is(n_str, rank + 1);
			// indexをorgのものに戻す
			for (auto &e : lms_sa)e = lms_pos[e];
		}

		// 再度induced_sortをlms_saに沿って行うとSuffixArrayが求まる
		return induced_sort(str, lms_sa, is_s, kind);
	}

	std::vector<bool> calcS(const std::vector<int> &str) {
		const int n = str.size();
		std::vector<bool>isS(n);
		isS[n - 1] = true;
		for (int i = n - 2; i >= 0; --i) {
			if (str[i] < str[i + 1])isS[i] = true;
			else if (str[i] == str[i + 1] && isS[i + 1])isS[i] = true;
		}
		return isS;
	}

	std::vector<bool> calcLMS(const std::vector<bool> &isS) {
		const int n = isS.size();
		std::vector<bool>isLMS(n);
		for (int i = 0; i < n; ++i) {
			if (isS[i] && (0 == i || !isS[i - 1])) {
				isLMS[i] = true;
			}
		}
		return isLMS;
	}

	std::vector<int> induced_sort(const std::vector<int> &str, const std::vector<int> &lms_pos, const std::vector<bool>&is_s, const int kind) {
		// dump(lms_pos);
		const int n = str.size();
		std::vector<int>buckets(n);

		// 出現する文字に対しての累積和
		std::vector<int>cum(kind + 1);
		for (auto e : str)cum[e + 1]++;
		for (int i = 0; i < kind; ++i)cum[i + 1] += cum[i];

		// 種類ごとの詰めた数
		std::vector<int>count(kind);

		// Step1
		// LMSを後ろから
		for (int i = lms_pos.size() - 1; i >= 0; --i) {
			int c = str[lms_pos[i]];
			int index = cum[c + 1] - count[c] - 1;
			buckets[index] = lms_pos[i];
			count[c]++;
		}
		// dump(buckets);

		// Step2
		// S[i-1]がLなものを前から
		count = std::vector<int>(kind);
		for (int i = 0; i < n; ++i) {
			// 未処理or番兵
			if (0 == buckets[i])continue;
			// notL
			if (is_s[buckets[i] - 1])continue;
			int c = str[buckets[i] - 1];
			int index = cum[c] + count[c];
			buckets[index] = buckets[i] - 1;
			count[c]++;
		}
		// dump(buckets);

		// Step3
		// S[i-1]がSなものを後ろから
		count = std::vector<int>(kind);
		for (int i = n - 1; i >= 0; --i) {
			// 未処理or番兵
			if (0 == buckets[i])continue;
			// notS
			if (!is_s[buckets[i] - 1])continue;
			int c = str[buckets[i] - 1];
			int index = cum[c + 1] - count[c] - 1;
			buckets[index] = buckets[i] - 1;
			count[c]++;
		}
		// dump(buckets);

		return buckets;
	}

	/*void dump(std::vector<int>vec) {
		for (auto e : vec)std::cout << e << " ";
		std::cout << std::endl;
	}*/
};
std::vector<int> suffixArray(const std::string &s) {
	SuffixArray sa = SuffixArray(s);
	return sa();
}
