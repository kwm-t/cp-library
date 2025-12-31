mint count_spanning_tree(std::vector<std::vector<mint>> mat) {
	//行列木定理
	mint ans = 1;
	for (int i = 0; i < (int)mat.size(); ++i) {
		if (0 == i)continue;
		ans *= mat[i][i];
		if (0 == mat[i][i]) continue;
		mint inv = inv_mod(mat[i][i].val(), mod);
		for (int j = 0; j < (int)mat.size(); ++j) {
			if (i == j) continue;
			mint num = mat[j][i] * inv;
			for (int k = 0; k < (int)mat.size(); ++k) mat[j][k] -= num * mat[i][k];
		}
	}
	return ans;
}