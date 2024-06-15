// (i,j)->(2 * i + 1,2 * j + 1)
std::vector<std::string>createGrid(int h, int w) {
	std::vector<std::string>board(h * 2 + 1);
	for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < w * 2 + 1; ++j)board[i] += "+";
		if (0 == i || i == board.size() - 1)continue;
		if (1 == i % 2) {
			for (int j = 1; j < w * 2; j++) {
				if (1 == j % 2)board[i][j] = 'o';
				else board[i][j] = '|';
			}
		}
		else {
			for (int j = 1; j < w * 2; j++) {
				if (1 == j % 2)board[i][j] = '-';
			}
		}
	}
	return board;
}