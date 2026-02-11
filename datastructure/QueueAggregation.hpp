// SWAG (Sliding Window Aggregation)

template <class T, class Op>
class QueueAggregation {
private:
	struct Node {
		T value;
		T agg; // aggregation result
		Node(const T& v, const T& a) : value(v), agg(a) {}
	};

	Op op;
	std::stack<Node> front, back;

	void move_back_to_front() {
		front.emplace(back.top().value, back.top().value);
		back.pop();
		while (!back.empty()) {
			T agg = op(back.top().value, front.top().agg);
			front.emplace(back.top().value, agg);
			back.pop();
		}
	}

public:
	explicit QueueAggregation(const Op& op = Op()) : op(op) {}

	bool empty() const {
		return front.empty() && back.empty();
	}

	size_t size() const {
		return front.size() + back.size();
	}

	T fold_all() const {
		assert(!empty());
		if (front.empty()) return back.top().agg;
		if (back.empty())  return front.top().agg;
		return op(front.top().agg, back.top().agg);
	}

	void push(const T& x) {
		if (back.empty()) back.emplace(x, x);
		else back.emplace(x, op(back.top().agg, x));
	}

	void pop() {
		assert(!empty());
		if (front.empty()) move_back_to_front();
		front.pop();
	}
};
