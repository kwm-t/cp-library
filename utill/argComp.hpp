template <class T>
bool argcmp(std::pair<T, T> p0, std::pair<T, T> p1) {
	auto[x0, y0] = p0;
	auto[x1, y1] = p1;
	bool b0 = (std::pair{ 0, 0 } < std::pair{ y0, x0 });
	bool b1 = (std::pair{ 0, 0 } < std::pair{ y1, x1 });
	if (b0 == b1)return x0 * y1 > x1 * y0;
	else return std::pair{ y0, x0 } > std::pair{ y1, x1 };
}