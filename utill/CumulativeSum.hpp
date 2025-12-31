// 2D累積和
template<typename T>
std::vector<std::vector<T>> create2D(const std::vector<std::vector<T>>& org) {
	int h = org.size(), w = org[0].size();
	std::vector res(h + 1, std::vector<T>(w + 1));
	for (int i = 0; i < h; ++i)for (int j = 0; j < w; ++j)res[i + 1][j + 1] = org[i][j];

	for (int i = 0; i < h + 1; ++i)for (int j = 0; j < w; ++j) res[i][j + 1] += res[i][j];
	for (int i = 0; i < h; ++i)for (int j = 0; j < w + 1; ++j) res[i + 1][j] += res[i][j];
	return res;
}

template<typename T>
T calc2D(const std::vector<std::vector<T>>& mat, int x0, int y0, int x1, int y1) {
	// min(x0,y0),max(領域外)(x1,y1)
	return  mat[x1][y1] - mat[x1][y0] - mat[x0][y1] + mat[x0][y0];
}

template<typename T>
std::vector<std::vector<std::vector<T>>> create3D(const std::vector<std::vector<std::vector<T>>>& org) {
	int h = org.size(), w = org[0].size(), d = org[0][0].size();
	std::vector res(h + 1, std::vector(w + 1, std::vector<T>(d + 1)));
	for (int i = 0; i < h; ++i)for (int j = 0; j < w; ++j)for (int k = 0; k < d; ++k)res[i + 1][j + 1][k + 1] = org[i][j][k];

	for (int i = 0; i <= h; ++i)for (int j = 0; j <= w; ++j)for (int k = 0; k < d; ++k)res[i + 0][j + 0][k + 1] += res[i][j][k];
	for (int i = 0; i <= h; ++i)for (int j = 0; j < w; ++j)for (int k = 0; k <= d; ++k)res[i + 0][j + 1][k + 0] += res[i][j][k];
	for (int i = 0; i < h; ++i)for (int j = 0; j <= w; ++j)for (int k = 0; k <= d; ++k)res[i + 1][j + 0][k + 0] += res[i][j][k];
	return res;
}

template<typename T>
T calc3D(const std::vector<std::vector<std::vector<T>>>& mat, int x0, int y0, int z0, int x1, int y1, int z1) {
	// min(x0,y0,z0),max(領域外)(x1,y1,z1)
	return  mat[x1][y1][z1] - mat[x1][y1][z0] - mat[x1][y0][z1] - mat[x0][y1][z1]
		+ mat[x0][y0][z1] + mat[x0][y1][z0] + mat[x1][y0][z0] - mat[x0][y0][z0];
}