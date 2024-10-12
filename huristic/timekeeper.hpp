class TimeKeeper {
private:
	std::chrono::high_resolution_clock::time_point start_time_;
	long long time_threshold_;
public:
	TimeKeeper(const long long &time_threshold)
		:start_time_(std::chrono::high_resolution_clock::now()), time_threshold_(time_threshold) {}
	
	bool isTimeOver() const {
		using std::chrono::duration_cast;
		using std::chrono::milliseconds;
		auto diff = std::chrono::high_resolution_clock::now() - start_time_;
		return duration_cast<milliseconds>(diff).count() >= time_threshold_;
	}
	
	long long getTime() {
		using std::chrono::duration_cast;
		using std::chrono::milliseconds;
		auto diff = std::chrono::high_resolution_clock::now() - start_time_;
		return duration_cast<milliseconds>(diff).count();
	}
};