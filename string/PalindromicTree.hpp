template<typename T = char>
struct PalindromicTree {
public:
	struct Node {
		std::map<T, int>link;	// child node index
		int suffixLink;			// 最長回文suffixのindex
		int length;				// 回分の長さ
		std::vector<int>index;	// suffixLinkをこのNodeに貼るstring上のindex
		int deltaLink;			// 差分が異なる最長回文接尾辞のindex

		Node() {}
		Node(int suffixLink_, int length_) :suffixLink(suffixLink_), length(length_), deltaLink(-1) {}
	};

	std::vector<Node>nodes;
	int pointer;
	std::vector<T>vs;

private:
	int find_prev_palindrome(int cur) const {
		int pos = (int)vs.size() - 1;
		while (true) {
			int rev = pos - nodes[cur].length - 1;
			if (rev >= 0 && vs[rev] == vs.back())break;
			cur = nodes[cur].suffixLink;
		}
		return cur;
	}
	int diff(int index) const {
		// 最長回文接尾辞との差分を返す
		if (nodes[index].suffixLink <= 0)return -1;
		return nodes[index].length - nodes[nodes[index].suffixLink].length;
	}
	// output_dfs
public:
	PalindromicTree() :pointer(0) {
		// dummyの頂点
		nodes.emplace_back(0, -1);
		nodes.emplace_back(0, 0);
	}
	PalindromicTree(const std::string &s) : PalindromicTree() {
		add(s);
	}
	void add(const std::string&s) {
		for (auto &c : s)add(c);
	}
	int add(const T &c) {
		int index = (int)vs.size();
		vs.push_back(c);
		int cur = find_prev_palindrome(pointer);
		auto res = nodes[cur].link.insert(std::make_pair(c, (int)nodes.size()));
		pointer = res.first->second;
		if (res.second) {// add new node
			nodes.emplace_back(-1, nodes[cur].length + 2);
			// set suffix link
			if (nodes.back().length == 1) {
				nodes.back().suffixLink = 1;
			}
			else {
				nodes.back().suffixLink = nodes[find_prev_palindrome(nodes[cur].suffixLink)].link[c];
			}
			if (diff(pointer) == diff(nodes.back().suffixLink)) {
				nodes.back().deltaLink = nodes[nodes.back().suffixLink].deltaLink;
			}
			else {
				nodes.back().deltaLink = nodes.back().suffixLink;
			}
		}
		nodes[pointer].index.emplace_back(index);
		return pointer;
	}
	// addの直後に一度呼ぶ想定
	template<typename I, typename U>
	std::vector<int>updateDp(const I &init, const U &apply) {
		int i = (int)vs.size() - 1;
		int id = pointer;
		std::vector<int>update;
		while (nodes[id].length > 0) {
			init(id, i + 1 - nodes[nodes[id].deltaLink].length - diff(id));
			if (nodes[id].suffixLink != nodes[id].deltaLink) {
				apply(id, nodes[id].suffixLink);
			}
			update.push_back(id);
			id = nodes[id].deltaLink;
		}
		return update;
	}
	// build_frequency
	// output
	int size()const { return(int)nodes.size(); }
	Node &operator[](int index) { return nodes[index]; }
};
// document
// https://math314.hateblo.jp/entry/2016/12/19/005919
// https://ei1333.github.io/library/string/palindromic-tree.hpp.html
// example
// https://judge.yosupo.jp/submission/270489
// https://yukicoder.me/submissions/1048061
// ??? 
// https://yukicoder.me/problems/no/273 
// https://yukicoder.me/problems/no/465 