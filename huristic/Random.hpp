class Random {
public:
	std::mt19937 mt_;
	// [0,1.0)
	std::uniform_real_distribution<double> dd_{ 0.0, 1.0 };
	// seed‚ğw’è‚µ‚Ä‰Šú‰»
	Random(const int seed = 0) : mt_(std::mt19937(seed)) {}
	// [0,m)‚Ì®”‚Ì”ÍˆÍ‚Ì—”
	inline int nextInt(const int m) {
		std::uniform_int_distribution<int> di(0, m - 1);
		return di(mt_);
	}
	// [n,m)‚Ì®”‚Ì”ÍˆÍ‚Ì—”
	inline int nextInt(const int n, const int m) {
		std::uniform_int_distribution<int> di(0, m - n - 1);
		return di(mt_) + n;
	}
	// [0,1.0)
	inline double nextDouble() { return dd_(mt_); }
	// [0,1.0)
	inline double nextLog() { return log(dd_(mt_)); }
};