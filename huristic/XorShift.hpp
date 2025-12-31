class Xorshift {
	uint64_t x;

public:
	using result_type = uint32_t;
	static constexpr result_type min() {
		return std::numeric_limits<result_type>::min();
	}
	static constexpr result_type max() {
		return std::numeric_limits<result_type>::max();
	}
	Xorshift(uint64_t k) : x(k) {}
	Xorshift() : Xorshift(1) {}
	result_type operator()() {
		x ^= x << 9;
		return x ^= x >> 7;
		// return (x * 0x123456789abcdef) >> 32;
	}
	int64_t operator()(int m) { return int64_t((*this)()) * m >> 32; }
	double rand() { return double((*this)()) / (int64_t(1) << 32); }
};