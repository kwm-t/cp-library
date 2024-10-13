#include <array>
#include <vector>
struct BinaryTrie {
	int kind;
	std::vector<int>node;
	std::vector<std::array<int, 2>>next;
	std::array<int, 2> e{ -1,-1 };
	BinaryTrie(int _kind) :kind(_kind) {
		node.resize(1);
		next.emplace_back(e);
	}
	void insert(const std::vector<int>&v) {
		int now = 0;
		for (int i = 0; i < v.size(); ++i) {
			if (-1 == next[now][v[i]]) {
				next[now][v[i]] = node.size();
				node.emplace_back(0);
				next.emplace_back(e);
			}
			now = next[now][v[i]];
			node[now]++;
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
		for (int i = 0; i < (int)v.size(); ++i) {
			if (-1 == next[now][v[i]])return false;
			if (node[next[now][v[i]]] <= 0)return false;
			now = next[now][v[i]];
		}
		int count = node[now];
		for (int i = 0; i < kind; ++i)if (-1 != next[now][i])count -= node[next[now][i]];
		return count;
	}
	void erase(const std::vector<int>&v) {
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			now = next[now][v[i]];
			node[now]--;
		}
	}
	// Binary Trie
	std::vector<int>xor_max(const std::vector<int>&v) {
		std::vector<int>ret;
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			std::vector<int>tmp = { 1 - v[i],v[i] };
			for (int j = 0; j < (int)tmp.size(); ++j) {
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
		for (int i = 0; i < (int)v.size(); ++i) {
			std::vector<int>tmp = { v[i],1 - v[i] };
			for (int j = 0; j < (int)tmp.size(); ++j) {
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
		for (int i = 0; i < (int)k.size(); ++i) {
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
	// xより大きいものの個数
	int over_count(const std::vector<int>&x) {
		int ret = 0, now = 0;
		for (int i = 0; i < (int)x.size(); ++i) {
			if (0 == x[i]) {
				if (-1 != next[now][1])ret += node[next[now][1]];
			}
			if (-1 == next[now][x[i]])break;
			now = next[now][x[i]];
		}
		return ret;
	}
	std::vector<int> trans(long long n, int sz = 30) {
		std::vector<int>ret(sz, 0);
		for (int i = 0; i < sz; ++i) {
			if (1 & (n >> i))ret[sz - 1 - i] = 1;
		}
		return ret;
	}
	long long inverse(const std::vector<int>&v) {
		long long ret = 0;
		for (int i = 0; i < v.size(); ++i) {
			ret *= 2;
			ret += v[i];
		}
		return ret;
	}
};