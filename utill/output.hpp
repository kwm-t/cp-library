template<typename T> void output(std::vector<T>v) {
	for (int i = 0; i < v.size(); ++i) {
		if (0 != i)std::cout << " ";
		std::cout << v[i];
	}
	std::cout << std::endl;
}