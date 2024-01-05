// 2D—ÝÏ˜a
// ¶ã(x0,y0)‰E‰º(—ÌˆæŠO)(x1,y1)
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