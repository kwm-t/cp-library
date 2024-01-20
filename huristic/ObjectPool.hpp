// メモリの再利用を行いつつ集合を管理するクラス
// 同時に使用する最大サイズ分を予めしておき
// 不要になった領域を再利用する
template<class T>
class ObjectPool {
public:
	T& operator[](int i) {
		return data_[i];
	}
	void reserve(size_t capacity) {
		data_.reserve(capacity);
	}
	int push(const T& x) {
		if (garbage_.empty()) {
			data_.push_back(x);
			return data_.size() - 1;
		}
		else {
			int i = garbage_.top();
			garbage_.pop();
			data_[i] = x;
			return i;
		}
	}
	void pop(int i) {
		garbage_.push(i);
	}
	size_t size() {
		return data_.size();
	}
private:
	std::vector<T> data_;
	std::stack<int> garbage_;
};