#include <vector>
#include <iostream>
auto query(std::vector<int>v, int add = 0) {
	std::cout << "?";
	for (int i = 0; i < v.size();++i)std::cout << " " << v[i] + add;
	std::cout << std::endl;
	fflush(stdout);
	int get; std::cin >> get;
	return get;
}
void answer(std::vector<int>v, int add = 0) {
	std::cout << "!";
	std::cout << " " << v.size();
	for (int i = 0; i < v.size(); ++i)std::cout << " " << v[i] + add;
	std::cout << std::endl;
	fflush(stdout);
}