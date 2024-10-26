#include <vector>
struct trie {
	int kind;
	std::vector<int>node;
	std::vector<std::vector<int>>next;
	std::vector<bool>end;
	trie(int _kind) :kind(_kind) {
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
				for (int j = 0; j < 2; ++j)if (-1 != next[now][j])ret -= node[next[now][j]];
			}
			if (0 == v[i]) {
				if (-1 == next[now][v[i]])break;
				now = next[now][v[i]];
			}
			else {
				if (-1 != next[now][0]) ret += node[next[now][0]];
				if (-1 == next[now][v[i]])break;
				now = next[now][v[i]];
			}
		}
		return ret;
	}
	// prefixも取得する
	std::vector<int> getIndexEx(const std::vector<int>&v) {
		std::vector<int>ret;
		int idx = 0, now = 0;
		for (int i = 0; i < v.size(); ++i) {
			if (0 != now) {
				idx += node[now];
				for (int j = 0; j < 2; ++j)if (-1 != next[now][j])idx -= node[next[now][j]];
			}
			if (0 == v[i]) {
				if (-1 == next[now][v[i]])break;
				now = next[now][v[i]];
			}
			else {
				if (-1 != next[now][0]) idx += node[next[now][0]];
				if (-1 == next[now][v[i]])break;
				now = next[now][v[i]];
			}
			ret.push_back(idx);
		}
		return ret;
	}
	// Binary Trie
	std::vector<int>xor_max(const std::vector<int>&v) {
		std::vector<int>ret;
		int now = 0;
		for (int i = 0; i < v.size(); ++i) {
			std::vector<int>tmp = { 1 - v[i],v[i] };
			for (int j = 0; j < tmp.size(); ++j) {
				if (-1 == next[now][tmp[j]])continue;
				if (node[next[now][tmp[j]]] <= 0)continue;
				ret.emplace_back(tmp[j]);
				now = next[now][tmp[j]];
				break;
			}
		}
		return ret;
	}
	std::vector<int>xor_min(const std::vector<int>&v) {
		std::vector<int>ret;
		int now = 0;
		for (int i = 0; i < v.size(); ++i) {
			std::vector<int>tmp = { v[i],1 - v[i] };
			for (int j = 0; j < tmp.size(); ++j) {
				if (-1 == next[now][tmp[j]])continue;
				if (node[next[now][tmp[j]]] <= 0)continue;
				ret.emplace_back(tmp[j]);
				now = next[now][tmp[j]];
				break;
			}
		}
		return ret;
	}
	// xとxorした結果がk未満になる物の数(0含む)
	int xor_count(const std::vector<int>&x, const std::vector<int>&k) {
		int ret = 0, now = 0;
		for (int i = 0; i < k.size(); ++i) {
			if (0 == k[i]) {
				if (-1 == next[now][x[i]])break;
				now = next[now][x[i]];
			}
			else {
				if (-1 != next[now][x[i]])ret += node[next[now][x[i]]];
				if (-1 == next[now][1 - x[i]])break;
				now = next[now][1 - x[i]];
			}
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