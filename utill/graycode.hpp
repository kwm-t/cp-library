#include <vector>
std::vector<int>gray(1 << n);
for (int i = 0; i << (1 << n); ++i)gray[i] = i ^ (i >> 1);