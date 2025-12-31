#include<algorithm>
#include <vector>
template<class T>
std::vector<std::pair<T, T>>convex_hull(std::vector<std::pair<T, T>>V) {
	// 注意！！
	// 同一直線上の三点を含む、除きたいときは >=にする
	std::sort(V.begin(), V.end());
	int n = V.size();
	std::vector<std::pair<T, T>>X;
	int k = 0;
	for (int i = 0; i < n; ++i) {
		while ((k >= 2) && ((V[i].first - X[k - 2].first)*(X[k - 1].second - X[k - 2].second) > (V[i].second - X[k - 2].second)*(X[k - 1].first - X[k - 2].first))) {
			X.pop_back();
			k--;
		}
		X.push_back(V[i]);
		k++;
	}
	std::vector<std::pair<double, double>>Y;
	int l = 0;
	for (int i = n - 1; i >= 0; --i) {
		while ((l >= 2) && ((V[i].first - Y[l - 2].first)*(Y[l - 1].second - Y[l - 2].second) > (V[i].second - Y[l - 2].second)*(Y[l - 1].first - Y[l - 2].first))) {
			Y.pop_back();
			l--;
		}
		Y.push_back(V[i]);
		l++;
	}
	std::vector<std::pair<T, T>>d;
	for (int i = 0; i < (int)X.size(); ++i)d.push_back(X[i]);
	for (int i = 1; i < (int)Y.size() - 1; ++i)d.push_back(Y[i]);
	return d;

	//周りの長さを返す
	/*double ret = 0;
	for (int i = 0; i < X.size() - 1; ++i) {
		ret += sqrt((X[i + 1].first - X[i].first)*(X[i + 1].first - X[i].first) + (X[i + 1].second - X[i].second)*(X[i + 1].second - X[i].second));
	}
	for (int i = 0; i < Y.size() - 1; ++i) {
		ret += sqrt((Y[i + 1].first - Y[i].first)*(Y[i + 1].first - Y[i].first) + (Y[i + 1].second - Y[i].second)*(Y[i + 1].second - Y[i].second));
	}
	return ret;*/
}
#define PL std::pair<long long,long long>
// index付き
std::vector<std::pair<PL, int>> convex_hull(std::vector<std::pair<PL, int>>v) {
	// 注意！！
	// 同一直線上の三点を含む、除きたいときは >=にする
	std::sort(v.begin(), v.end());
	int n = v.size();
	std::vector<std::pair<PL, int>>x;
	int k = 0;
	for (int i = 0; i < n; ++i) {
		while ((k >= 2) &&
			((v[i].first.first - x[k - 2].first.first)*(x[k - 1].first.second - x[k - 2].first.second)
				>= (v[i].first.second - x[k - 2].first.second)*(x[k - 1].first.first - x[k - 2].first.first))) {
			x.pop_back();
			k--;
		}
		x.push_back(v[i]);
		k++;
	}

	std::vector<std::pair<PL, int>>y;
	int l = 0;
	for (int i = n - 1; i >= 0; --i) {
		while ((l >= 2) &&
			((v[i].first.first - y[l - 2].first.first)*(y[l - 1].first.second - y[l - 2].first.second)
				>= (v[i].first.second - y[l - 2].first.second)*(y[l - 1].first.first - y[l - 2].first.first))) {
			y.pop_back();
			l--;
		}
		y.push_back(v[i]);
		l++;
	}
	std::vector<std::pair<PL, int>>ret;
	for (int i = 0; i < (int)x.size(); ++i)ret.push_back(x[i]);
	for (int i = 1; i < (int)y.size() - 1; ++i)ret.push_back(y[i]);
	return ret;
}