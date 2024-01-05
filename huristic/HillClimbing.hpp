#include <bits/stdc++.h>
#include <atcoder/all>
//using namespace std;
using namespace atcoder;
//using mint = modint1000000007;
//const int mod = 1000000007;
//using mint = modint998244353;
//const int mod = 998244353;
//const int INF = 1e9;
//const long long LINF = 1e18;
// 時間管理
class TimeKeeperDouble {
private:
	std::chrono::high_resolution_clock::time_point start_time_;
	double time_threshold_;
	double now_time_ = 0;
public:
	TimeKeeperDouble(const double time_threshold)
		: start_time_(std::chrono::high_resolution_clock::now()),
		time_threshold_(time_threshold) {}

	void setNowTime() {
		auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
		this->now_time_ =
			std::chrono::duration_cast<std::chrono::microseconds>(diff).count() *
			1e-3;  // ms
	}

	double getNowTime() const { return this->now_time_; }

	bool isTimeOver() const { return now_time_ >= time_threshold_; }
};
// 時間管理
class TimeKeeper {
private:
	std::chrono::high_resolution_clock::time_point start_time_;
	long long time_threshold_;
public:
	TimeKeeper(const long long &time_threshold)
		:start_time_(std::chrono::high_resolution_clock::now()), time_threshold_(time_threshold) {}
	bool isTimeOver() const {
		using std::chrono::duration_cast;
		using std::chrono::milliseconds;
		auto diff = std::chrono::high_resolution_clock::now() - start_time_;
		return duration_cast<milliseconds>(diff).count() >= time_threshold_;
	}
	long long getTime() {
		using std::chrono::duration_cast;
		using std::chrono::milliseconds;
		auto diff = std::chrono::high_resolution_clock::now() - start_time_;
		return duration_cast<milliseconds>(diff).count();
	}
};
class Random {
public:
	std::mt19937 mt_;
	// [0,1.0)
	std::uniform_real_distribution<double> dd_{ 0.0, 1.0 };

	// seedを指定して初期化
	Random(const int seed = 0) : mt_(std::mt19937(seed)) {}

	// [0,m)の整数の範囲の乱数
	inline int nextInt(const int m) {
		std::uniform_int_distribution<int> di(0, m - 1);
		return di(mt_);
	}

	// [n,m)の整数の範囲の乱数
	inline int nextInt(const int n, const int m) {
		std::uniform_int_distribution<int> di(0, m - n - 1);
		return di(mt_) + n;
	}

	// [0,1.0)
	inline double nextDouble() { return dd_(mt_); }

	// [0,1.0)
	inline double nextLog() { return log(dd_(mt_)); }
};
class Input {
public:
	int n, k, l;// 50,400,20;
	std::vector<int>a, b;
	std::vector<std::vector<int>>c;
	Input(int n, int k, int l) :n(n), k(k), l(l) {
		a.resize(k);
		b.resize(k);
		c = std::vector<std::vector<int>>(n, std::vector<int>(n));
	}
	void readData() {
		for (int i = 0; i < k; ++i) {
			std::cin >> a[i] >> b[i];
		}
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				std::cin >> c[i][j];
				c[i][j]--;
			}
		}
	}
};
class Solver {
public:
	Solver(Input input) :input(input) {
		answer.resize(input.k);
	}
	void init() {
		createGraph();
	}
	void output() {
		for (int i = 0; i < answer.size(); ++i) {
			std::cout << answer[i] + 1 << std::endl;
		}
	}
	std::vector<int> getAnswer() {
		return answer;
	}
	void localDebug() {
		auto score = calcScore();
		outputScore(score);
	}
protected:
	const int TIME_THRESHOLD = 950;
	Input input;
	std::vector<std::vector<int >> graph;
	std::vector<int>answer;
	void createGraph() {
		graph.resize(input.k);
		for (int i = 0; i < input.n; ++i) {
			for (int j = 0; j < input.n; ++j) {
				if (input.n - 1 != i) {
					const int ni = i + 1;
					const int nj = j + 0;
					if (-1 != input.c[i][j] && -1 != input.c[ni][nj] && input.c[i][j] != input.c[ni][nj]) {
						graph[input.c[i][j]].push_back(input.c[ni][nj]);
						graph[input.c[ni][nj]].push_back(input.c[i][j]);
					}
				}
				if (input.n - 1 != j) {
					const int ni = i + 0;
					const int nj = j + 1;
					if (-1 != input.c[i][j] && -1 != input.c[ni][nj] && input.c[i][j] != input.c[ni][nj]) {
						graph[input.c[i][j]].push_back(input.c[ni][nj]);
						graph[input.c[ni][nj]].push_back(input.c[i][j]);
					}
				}
			}
		}
		for (int i = 0; i < input.k; ++i) {
			std::sort(graph[i].begin(), graph[i].end());
			graph[i].erase(std::unique(graph[i].begin(), graph[i].end()), graph[i].end());
		}
	}
	void dfs(int pos, std::vector<bool>&visited) {
		visited[pos] = true;
		for (int i = 0; i < (int)graph[pos].size(); ++i) {
			int npos = graph[pos][i];
			if (visited[npos])continue;
			if (answer[pos] == answer[npos])dfs(npos, visited);
		}
	}
	double calcScore() {
		std::vector<bool>visited(input.k);
		for (int i = 0; i < input.l; ++i) {
			int pos = -1;
			for (int j = 0; j < input.k; ++j) {
				if (answer[j] == i) {
					pos = j;
					break;
				}
			}
			if (-1 == pos)return 0.0;
			dfs(pos, visited);
		}
		for (int i = 0; i < input.k; ++i) {
			if (!visited[i])return 0.0;
		}
		std::vector<int>p(input.l);
		std::vector<int>q(input.l);
		for (int i = 0; i < input.k; ++i) {
			p[answer[i]] += input.a[i];
			q[answer[i]] += input.b[i];
		}
		int minP = *std::min_element(p.begin(), p.end());
		int maxP = *std::max_element(p.begin(), p.end());
		int minQ = *std::min_element(q.begin(), q.end());
		int maxQ = *std::max_element(q.begin(), q.end());
		return std::min(double(minP) / maxP, double(minQ) / maxQ);
	}
	void outputScore(double score) {
		std::cout << std::setprecision(16) << score << std::endl;
	}
};
class NaiveSolver :public Solver {
public:
	// dsuを用いて分割された後のサイズをなるべく均等にする
	NaiveSolver(Input input) :Solver(input) {}
	void run() {
		dsu uf(input.k);
		for (int i = 0; i < input.k - input.l; ++i) {
			int mnSize = 1e9, v0 = -1, v1 = -1;
			for (int j = 0; j < input.k; ++j) {
				for (int k = 0; k < (int)graph[j].size(); ++k) {
					int v = graph[j][k];
					if (uf.same(j, v)) continue;
					int size0 = uf.size(j);
					int size1 = uf.size(v);
					if (mnSize > size0 + size1) {
						mnSize = size0 + size1;
						v0 = j;
						v1 = v;
					}
				}
			}
			uf.merge(v0, v1);
		}
		auto groups = uf.groups();
		std::vector<int>root(input.k);
		for (int i = 0; i < groups.size(); ++i) {
			root[groups[i][0]] = i;
		}
		for (int i = 0; i < groups.size(); ++i) {
			int r = root[groups[i][0]];
			for (int j = 0; j < groups[i].size(); ++j) {
				answer[groups[i][j]] = r;
			}
		}
	}
};
class HillClimbingSolver : public Solver {
public:
	HillClimbingSolver(Input input) :Solver(input) {}
	void run() {
		auto time_keeper = TimeKeeper(TIME_THRESHOLD);
		createInitialAnswer();
		double nowScore = calcScore();
		Random rnd;
		while (true) {
			if (time_keeper.isTimeOver())break;
			int v, x;
			do {
				v = rnd.nextInt(input.k);
				x = answer[graph[v][rnd.nextInt((int)graph[v].size())]];
			} while (answer[x] == answer[v]);
			int oldX = answer[v];
			answer[v] = x;
			double newScore = calcScore();
			if (0.0 != newScore && newScore >= nowScore) {
				nowScore = newScore;
			}
			else {
				answer[v] = oldX;
			}
		}
	}
private:
	void createInitialAnswer() {
		// 初期解
		NaiveSolver solver(input);
		solver.init();
		solver.run();
		answer = solver.getAnswer();
	}
};
class SimulatedAnnealingSolver : public Solver {
public:
	SimulatedAnnealingSolver(Input input) :Solver(input) {}
	void run(const double start_temp = 0.0040, const double end_temp = 0) {
		auto time_keeper = TimeKeeperDouble(TIME_THRESHOLD);
		createInitialAnswer();
		double nowScore = calcScore();
		Random rnd;
		while (true) {
			time_keeper.setNowTime();
			if (time_keeper.isTimeOver())break;
			int v, x;
			do {
				v = rnd.nextInt(input.k);
				x = answer[graph[v][rnd.nextInt((int)graph[v].size())]];
			} while (answer[x] == answer[v]);
			int oldX = answer[v];
			answer[v] = x;
			double newScore = calcScore();
			double diff = newScore - nowScore;
			double temp = start_temp + (end_temp - start_temp) * (time_keeper.getNowTime() / TIME_THRESHOLD);
			if (0.0 != newScore && exp(diff / temp) > rnd.nextDouble()) {
				nowScore = newScore;
			}
			else {
				answer[v] = oldX;
			}
		}
	}
private:
	void createInitialAnswer() {
		// 初期解
		NaiveSolver solver(input);
		solver.init();
		solver.run();
		answer = solver.getAnswer();
	}
};

// NaiveSolver				43128693
// HillClimbingSolver		88264583->88332089
// SimulatedAnnealingSolver	
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int n, k, l; std::cin >> n >> k >> l;
	Input input(n, k, l);
	input.readData();
	if (bool debug = false) {
		{
			NaiveSolver solver(input);
			solver.init();
			solver.run();
			solver.localDebug();
		}
		{
			HillClimbingSolver solver(input);
			solver.init();
			solver.run();
			solver.localDebug();
			//solver.output();
		}
		{
			SimulatedAnnealingSolver solver(input);
			solver.init();
			solver.run();
			solver.localDebug();
			//solver.output();
		}
	}
	else {
		SimulatedAnnealingSolver solver(input);
		solver.init();
		solver.run();
		solver.output();
	}
	return 0;
}