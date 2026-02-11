// double
printf("%.16f\n", ans);
printf("%.16Lf\n", ans);
cout << fixed << setprecision(16) << endl;

//再帰じゃなくて下から見るタイプ(i=j,i=0は含まれないのに注意)
rep(i, 1 << n) {
	for (int j = (i - 1)&i; j > 0; j = (j - 1)&i) {
		chmin(dp[i], dp[j] + dp[i^j]);
	}
}

//代表を決めるタイプ(代表側に偏る場合は別処理)
int bit = i & -i;
for (int j = (i - 1)&i; j > 0; j = (j - 1)&i) {
	if (j & bit) dp[i] += dp[i^j] * h[j] * f[popcount(j)];
}

// 文字列を小文字に変換
transform(S.begin(), S.end(), S.begin(), [](char c) {return tolower(c); });

// 順列
do {
} while (next_permutation(v.begin(), v.end()));

//[l0,r0]と[l1,r1]の共通範囲の長さを簡単に求める
 clamp(r1, l0, r0) - clamp(l0, l0, r0);
 
 
const int dx[4] = { 1,0,-1,0 };
const int dy[4] = { 0,-1,0,1 };
const int dx[8] = { 1,0,-1,0,1,1,-1,-1 };
const int dy[8] = { 0,-1,0,1,1,-1,1,-1 };



#ifdef _MSC_VER
using int128 = long long;
#else
using int128 = __int128;// 提出用
#endif