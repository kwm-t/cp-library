#include "bits/stdc++.h"
using namespace std;
//はxor ver
vector<vector<mint>> mul(const vector<vector<mint>>&A,const vector<vector<mint>>&B) {
	vector<vector<mint>> C(A.size(), vector<mint>(B[0].size()));
	rep(i, A.size()) rep(j, B[0].size()) rep(k, A[0].size()) C[i][j] += A[i][k] * B[k][j];
	//rep(i, A.size()) rep(j, B[0].size()) rep(k, A[0].size()) C[i][j] += A[i][k] & B[k][j];
	return C;
}
template<typename T>
vector<vector<T>> mul(const vector<vector<T>>&A,const vector<vector<T>>&B) {
	vector<vector<T>> C(A.size(), vector<T>(B[0].size()));
	for (int i = 0; i < (int)A.size(); ++i) {
		for (int j = 0; j < (int)B.size(); ++j) {
			for (int k = 0; k < (int)C.size(); ++k) C[i][j] += A[i][k] * B[k][j];
		}
	}
	return C;
}

int main() {
	int k, m;
	cin >> k >> m;
	m--;
	vector<vector<mint>> mans(k, vector<mint>(k));
	rep(i,k)mans[i][i] = 1;
	vector<vector<mint>> mpow(k, vector<mint>(k));
	//mpow初期化
	while (n > 0) {
		if (1 == n%2) mans = mul(mpow, mans);
		mpow = mul(mpow,mpow);
		n /= 2;
	}
	cout << mans[0][0]<<endl;
	return 0;
}