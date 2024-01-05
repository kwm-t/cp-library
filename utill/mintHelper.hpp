#include<iostream>
void esperHelper(mint m, double d) {
	printf("%.16f\n", d);
	const double EPS = 1e-10;
	if (0 == m) {
		std::cout << "0/1" << std::endl;
		return;
	}
	mint invm = inv_mod(m.val(), mod);
	for (int i = 1; i < 100000000; ++i) {
		mint j = (mint)i * invm;
		double dx = (double)i / (double)j.val();
		if (abs(dx - d) < EPS) {
			std::cout << i << "/" << j.val() << std::endl;
			return;
		}
	}
	for (int j = 1; j < 100000000; ++j) {
		mint i = (mint)j * m;
		double dx = (double)i.val() / (double)j;
		if (abs(dx - d) < EPS) {
			std::cout << i.val() << "/" << j << std::endl;
			return;
		}
	}
	std::cout << "not found" << std::endl;
	return;
}