#include "bits/stdc++.h"

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n; cin >> n;
	//★★doubleのpairみたいなもん
	complex<double>c;
	//★原点を中心に回している★
	//★回転して一致させたい★
	rep(i, n) rep(j, n) {
		if (i == j) continue;
		//★verの長さ比較★
		if (abs(abs(v0[1] - v0[0]) - abs(v1[j] - v1[i])) > EPS)continue;
		//★回転角★
		complex<double> x = (v1[j] - v1[i]) / (v0[1] - v0[0]);
		rep(k, n) {
			//★回転先★
			complex<double> nv0 = v0[k] * x + v1[i];
			bool b = false;
			//★一致判定★
			if (abs(nv0 - v1[l]) < EPS){
				b = true;
			}
		}
	}
	return 0;
}