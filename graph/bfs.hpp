#include <vector>
#include <queue>
const long long LINF = 1e18;

template<typename T>
struct bfs {// On_Grid
	const int dx[4] = { 1,0,-1,0 };
	const int dy[4] = { 0,-1,0,1 };
	const char c[4] = { 'D','L','U','R' };
	// const char c[] = { 'v','<','^','>' };
	bfs(std::vector<std::string>s) :s(s) {}
	std::vector<std::string> s;
	T solve(int h, int w, std::pair<int, int>pS, std::pair<int, int>pE) {
		std::vector<std::vector<T>> dp(h, std::vector<int>(w, LINF));
		std::queue<std::pair<int, int>> q;
		q.emplace(pS.first, pS.second);
		dp[pS.first][pS.second] = 0;
		while (!q.empty()) {
			int x = q.front().first;
			int y = q.front().second;
			q.pop();
			for (int i = 0; i < 4; ++i) {
				int nx = x + dx[i];
				int ny = y + dy[i];
				if ((0 <= nx) && (h > nx) && (0 <= ny) && (w > ny) && ('#' != s[nx][ny])) {
					if (chmin(dp[nx][ny], dp[x][y] + 1)) q.emplace(nx, ny);
				}
			}
		}
		if (LINF == dp[pE.first][pE.second]) return -1;
		return dp[pE.first][pE.second];
	}
};