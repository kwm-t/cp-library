#include<algorithm>
#include<cassert>
#include<vector>
template <class S>
struct Compression : std::vector<S> {
	bool built = false;
	using VS = std::vector<S>;
	using VS::VS;
	Compression(std::vector<S> v = {}) : std::vector<S>(move(v)) {}
	void build() {
		std::sort(this->begin(), this->end());
		this->erase(unique(this->begin(), this->end()), this->end());
		built = true;
	}
	template <class T>
	void convert(T first, T last) {
		assert(built);
		for (; first != last; ++first) *first = (*this)(*first);
	}
	int operator()(S x) {
		assert(built);
		return lower_bound(this->begin(), this->end(), x) - this->begin();
	}
	void clear() {
		this->clear();
		built = false;
	}
};

// 関数ver
template<typename T>
std::pair<std::vector<T>, int> compression(std::vector<T> org) {
	auto copy = org;
	std::sort(copy.begin(), copy.end());
	copy.erase(unique(copy.begin(), copy.end()), copy.end());
	for (auto &e : org) {
		e = std::lower_bound(copy.begin(), copy.end(), e) - copy.begin();
	}
	return { org,copy.size() };
}