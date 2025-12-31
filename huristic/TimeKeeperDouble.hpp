class TimeKeeperDouble {
private:
	std::chrono::high_resolution_clock::time_point start_time_;
	double time_threshold_;
	double now_time_ = 0;
public:
	TimeKeeperDouble(const double time_threshold)
		: start_time_(std::chrono::high_resolution_clock::now()),
		time_threshold_(time_threshold) {}
	void setNowTime() {
		auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
		this->now_time_ = std::chrono::duration_cast<std::chrono::microseconds>(diff).count() * 1e-3;
	}
	double getNowTime() const { return this->now_time_; }
	bool isTimeOver() const { return now_time_ >= time_threshold_; }
};