class TimeKeeper {
private:
	std::chrono::high_resolution_clock::time_point start_time_;
	std::chrono::high_resolution_clock::time_point before_time_;
	int64_t time_threshold_;
	int64_t end_turn_;
	int64_t turn_;

public:
	TimeKeeper(const int64_t &time_threshold, const int64_t end_turn)
		: start_time_(std::chrono::high_resolution_clock::now()),
		time_threshold_(time_threshold),
		end_turn_(end_turn),
		turn_(0) {
		before_time_ = start_time_;
	}

	void setTurn(const int t) {
		turn_ = t;
		this->before_time_ = std::chrono::high_resolution_clock::now();
	}

	bool isTimeOver() {
		auto now = std::chrono::high_resolution_clock::now();
		auto whole_diff = now - this->start_time_;
		auto whole_count =
			std::chrono::duration_cast<std::chrono::milliseconds>(whole_diff)
			.count();
		auto last_diff = now - this->before_time_;
		auto last_count =
			std::chrono::duration_cast<std::chrono::milliseconds>(last_diff)
			.count();

		auto remaining_time = time_threshold_ - whole_count;
		auto now_threshold = remaining_time / (end_turn_ - this->turn_);
		return last_count >= now_threshold;
	}
};