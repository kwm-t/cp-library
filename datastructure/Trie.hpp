#include <vector>
class Trie {
private:
	int kind;
	std::vector<int>node;
	std::vector<std::vector<int>>next;
	std::vector<bool>end;
public:
	Trie(int _kind) :kind(_kind) {
		node.resize(1);
		end.resize(1);
		next.emplace_back(std::vector<int>(kind, -1));
	}
	void insert(const std::vector<int>&v) {
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			if (-1 == next[now][v[i]]) {
				next[now][v[i]] = node.size();
				node.emplace_back(0);
				end.push_back(false);
				next.emplace_back(std::vector<int>(kind, -1));
			}
			now = next[now][v[i]];
			node[now]++;
		}
		end[now] = true;
	}
	// prefixも投入する
	void insertEx(const std::vector<int>&v) {
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			if (-1 == next[now][v[i]]) {
				next[now][v[i]] = node.size();
				node.emplace_back(0);
				next.emplace_back(std::vector<int>(kind, -1));
			}
			now = next[now][v[i]];
			node[now] += v.size() - i;
			end[now] = true;
		}
	}
	// 1-indexed
	std::vector<int>kth_element(int k) {
		int now = 0;
		std::vector<int>ret;
		while (true) {
			bool flag = false;
			for (int i = 0; i < kind; ++i) {
				if (-1 == next[now][i])continue;
				if (k <= node[next[now][i]]) {
					flag = true;
					ret.emplace_back(i);
					now = next[now][i];
					break;
				}
				else {
					k -= node[next[now][i]];
				}
			}
			if (!flag)break;
		}
		return ret;
	}
	bool seach(const std::vector<int>&v) {
		int now = 0;
		for (int i = 0; i < v.size(); ++i) {
			if (-1 == next[now][v[i]])return false;
			if (node[next[now][v[i]]] <= 0)return false;
			now = next[now][v[i]];
		}
		return end[now];
	}
	void erase(const std::vector<int>&v) {
		int now = 0;
		for (int i = 0; i < v.size(); ++i) {
			now = next[now][v[i]];
			node[now]--;
		}
		end[now] = false;
	}
	// GetIndex
	int getIndex(const std::vector<int>&v) {
		int ret = 0, now = 0;
		for (int i = 0; i < v.size(); ++i) {
			if (0 != now) {
				ret += node[now];
				for (int j = 0; j < kind; ++j)if (-1 != next[now][j])ret -= node[next[now][j]];
			}
			for (int j = 0; j < v[i]; ++j) {
				if (-1 != next[now][j]) ret += node[next[now][j]];
			}
			if (-1 == next[now][v[i]])break;
			now = next[now][v[i]];
		}
		return ret;
	}
	std::vector<int> trans(long long n, int sz = 30) {
		std::vector<int>ret(sz, 0);
		for (int i = 0; i < sz; ++i) {
			ret[sz - 1 - i] = n % kind;
			n /= kind;
		}
		return ret;
	}
	long long inverse(const std::vector<int>&v) {
		long long ret = 0;
		for (int i = 0; i < v.size(); ++i) {
			ret *= kind;
			ret += v[i];
		}
		return ret;
	}
};
// tire木上にvalueを持たせたいときは以下を参考に
// https://atcoder.jp/contests/abc377/submissions/59182087