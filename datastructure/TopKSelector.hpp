#include <vector>
template<typename T>
class TopKSelector {
public:
	TopKSelector(int k) :k(k) {
		vector.reserve(k * 2);
	}
	void push(T value) {
		if (sorted && vector[k - 1] <= value) {
			return;
		}
		vector.push_back(value);
		if (vector.size() >= k * 2) {
			std::sort(vector.begin(), vector.end());
			vector.resize(k);
			sorted = true;
		}
	}
	std::vector<T> get() {
		std::sort(vector.begin(), vector.end());
		vector.resize(k);
		return vector;
	}
private:
	int k;
	std::vector<T>vector;
	bool sorted = false;
};