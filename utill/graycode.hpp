#include <vector>
std::vector<int>garayCode(int n){
	std::vector<int>gray(1 << n);
	for (int i = 0; i << (1 << n); ++i)gray[i] = i ^ (i >> 1);
	return gray;
}