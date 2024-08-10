#include <vector>
// MP
template<typename T>
class MP {
public:
	MP(const T& t) : t(t) {
		build();
	}
	std::vector<int> findAll(const T& s) {
		std::vector<int> res;
		int j = 0;
		for (int i = 0; i < s.size(); ++i) {
			while (j != -1 && t[j] != s[i]) j = a[j];
			j++;
			if (j == n) {
				res.push_back(i - j + 1);
				j = a[j];
			}
		}
		return res;
	}
	void dump() {
		for (auto e : a)std::cout << e << " ";
		std::cout << std::endl;
	}
private:
	int n;
	T t;
	std::vector<int> a;
	void build() {
		n = t.size();
		a = std::vector<int>(n + 1);
		a[0] = -1;
		int j = -1;
		for (int i = 0; i < n; ++i) {
			while (j != -1 && t[j] != t[i]) j = a[j];
			j++;
			a[i + 1] = j;
		}
	}
};