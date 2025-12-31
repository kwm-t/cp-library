#include <algorithm>
#include <vector>
#include <queue>
const long long LINF = 1e18;
struct SlopeTrick {
	//コンストラクタ
	SlopeTrick() :minf(0), addR(0), addL(0) {}

	//max(0,x-a)を加算する
	void pushR(long long a) {
		if (0 == Lque.size()) {
			Rque.push(a - addR);
		}
		else {
			long long l0 = Lque.top() + addL;
			Lque.push(a - addL);
			long long x = Lque.top() + addL;
			Lque.pop();
			Rque.push(x - addR);
			minf += std::max((long long)0, l0 - a);
		}
	}
	//max(0,a-x)を加算する
	void pushL(long long a) {
		if (0 == Rque.size()) {
			Lque.push(a - addL);
		}
		else {
			long long r0 = Rque.top() + addR;
			Rque.push(a - addR);
			long long x = Rque.top() + addR;
			Rque.pop();
			Lque.push(x - addL);
			minf += std::max((long long)0, a - r0);
		}
	}
	//max(x-a,a-x)を加算する
	void push(long long a) { pushR(a); pushL(a); }
	//定数追加
	void addmin(long long a) { minf += a; }
	//傾きが0の区間の両端
	std::pair<long long, long long> getZero() {
		std::pair<long long, long long>p = { -LINF,LINF };
		if (0 != Lque.size()) { p.first = Lque.top() + addL; }
		if (0 != Rque.size()) { p.second = Rque.top() + addR; }
		return p;
	}
	//累積min
	void CumulativeMin() { Rque = std::priority_queue<long long, std::vector<long long>, std::greater<long long>>(); }
	//右側累積min
	void CumulativeMinR() { Lque = std::priority_queue<long long>(); }
	//関数の復元(遅い。非推奨？)
	std::vector<long long> Fx(std::vector<long long> &X) {
		std::vector<long long>vL;
		std::vector<long long>vR;
		std::vector<long long>ret(X.size());
		while (!Lque.empty()) {
			vL.push_back(Lque.top() + addL);
			Lque.pop();
		}
		while (!Rque.empty()) {
			vR.push_back(Rque.top() + addR);
			Rque.pop();
		}
		for (int i = 0; i < (int)X.size(); ++i) {
			long long num = minf;
			for (int j = 0; j < (int)vL.size(); ++j) { num += std::max((long long)0, vL[j] - X[i]); }
			for (int j = 0; j < (int)vR.size(); ++j) { num += std::max((long long)0, X[i] - vR[j]); }
			X[i] = num;
		}
		//元に戻す必要があれば
		//for (int i = 0; i < (int)vL.size(); ++i) { Lque.push(vL[i] - addL); }
		//for (int i = 0; i < (int)vR.size(); ++i) { Rque.push(vR[i] - addR); }
		return ret;
	}
	//関数の復元(遅い。非推奨？)
	long long FxSimple(long long X) {
		std::vector<long long>vL;
		std::vector<long long>vR;
		long long ret = minf;
		while (!Lque.empty()) {
			ret += std::max((long long)0, (Lque.top() + addL) - X);
			Lque.pop();
		}
		while (!Rque.empty()) {
			ret += std::max((long long)0, X - (Rque.top() + addR));
			Rque.pop();
		}
		return ret;
	}
	//右側スライド
	void SlideR(long long x) { addR += x; }
	//左側スライド
	void SlideL(long long x) { addL += x; }
	//全体スライド(無制限)
	void Slid(long long x) { SlideL(x), SlideR(x); };
	//全体スライド(a <= bでないと壊れる)
	//g(x) = min f(y) y∈[x-b,x-a]
	void Slid(long long b, long long a) { SlideL(a), SlideR(b); };
	//マージ(マージ元は以降使えなくなる。)
	void merge(SlopeTrick &st) {
		if ((Lque.size() + Rque.size()) < (st.Lque.size() + st.Rque.size())) {
			std::swap(Lque, st.Lque);
			std::swap(Rque, st.Rque);
			std::swap(minf, st.minf);
			std::swap(addR, st.addR);
			std::swap(addL, st.addL);
		}
		while (!st.Lque.empty()) {
			pushL(st.Lque.top() + st.addL);
			st.Lque.pop();
		}
		while (!st.Rque.empty()) {
			pushR(st.Rque.top() + st.addR);
			st.Rque.pop();
		}
		minf += st.minf;
	}

	std::priority_queue<long long> Lque;	//左側(大きい順)
	std::priority_queue<long long, std::vector<long long>, std::greater<long long>> Rque;	//右側(小さい順)
	long long minf;					//最小値
	long long addR;					//右側集合全てにたされる数
	long long addL;					//左側集合全てにたされる数
};