// 2D累積和
// 左上(x0,y0)右下(領域外)(x1,y1)
auto calc = [&](int x0, int y0, int x1, int y1)->long long {
	return  sa[x1][y1] - sa[x1][y0] - sa[x0][y1] + sa[x0][y0];
};
auto calc_debug= [&](int x0, int y0, int x1, int y1)->long long {
	long long val = calc(x0,y0,x1,y1);
	std::cout << x0 <<" " << y0 << " " << x1 <<" " << y1 << std::endl;
	std::cout << val << std::endl;
	return val;	
}
rep(i, n)rep(j, m)if ('1' == s[i][j])b[i + 1][j + 1]++;
rep(i, n + 1)rep(j, m) b[i][j + 1] += b[i][j];
rep(i, m + 1)rep(j, n) b[j + 1][i] += b[j][i];


// 3D
for (int i = 0; i < n; ++i)for (int j = 0; j < n; ++j)for (int k = 0; k < n; ++k)b[i + 1][j + 1][k + 1] = a[i][j][k];
for (int i = 0; i <= n; ++i)for (int j = 0; j <= n; ++j)for (int k = 0; k < n; ++k)b[i + 0][j + 0][k + 1] += b[i][j][k];
for (int i = 0; i <= n; ++i)for (int j = 0; j < n; ++j)for (int k = 0; k <= n; ++k)b[i + 0][j + 1][k + 0] += b[i][j][k];
for (int i = 0; i < n; ++i)for (int j = 0; j <= n; ++j)for (int k = 0; k <= n; ++k)b[i + 1][j + 0][k + 0] += b[i][j][k];
auto calcND = [&](int x0, int y0, int z0, int x1, int y1, int z1)->long long {
	return  b[x1][y1][z1] - b[x1][y1][z0] - b[x1][y0][z1] - b[x0][y1][z1]
		+ b[x0][y0][z1] + b[x0][y1][z0] + b[x1][y0][z0] - b[x0][y0][z0];
};