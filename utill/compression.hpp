template<typename T>
std::pair<std::vector<T>, int> compression(std::vector<T>org) {
	auto copy = org;
	std::sort(copy.begin(), copy.end());
	copy.erase(unique(copy.begin(), copy.end()), copy.end());
	for (auto &e : org) {
		e = std::lower_bound(copy.begin(), copy.end(), e) - copy.begin();
	}
	return { org,copy.size() };
}