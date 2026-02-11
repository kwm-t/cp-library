vector<int> v = {1, 4, 4, 7, 7, 8, 8, 11, 13, 19};
int target;
//特定の物の個数
int count = upper_bound(all(v), target) - lower_bound(all(v), target);
//以下の数
int count = upper_bound(all(v), target) - v.begin();
//以上の数
int count = v.end() - lower_bound(all(v), target);
//より小さいの数
int count = lower_bound(all(v), target)- v.begin();
//より大きいの数
int count = v.end()- lower_bound(all(v), target+1);
// [l,r)
int count = lower_bound(all(v), r)-lower_bound(all(v), l);

//set
set<int>st;
// 門番が必須
auto before = [&](long long n)->long long {
	auto ite = prev(st.lower_bound(n));
	return *ite;
};
auto after = [&](long long n)->long long {
	auto ite = st.upper_bound(n);
	return *ite;
};
// false:x以下(<=) / true:x未満(<)
auto left = [&](const auto& s, auto x, bool strict = false) {
	if (strict) return *prev(s.lower_bound(x));
	else return *prev(s.upper_bound(x));
	;

// false:x以上(>=) / true:xより大きい(>)
auto right = [&](const auto& s, auto x, bool strict = false) {
	if (strict) return *s.upper_bound(x);
	else return *s.lower_bound(x);
};



// lowerboundは
// 挿入するときのitrを返す

targetの次に大きな物
*upper_bound(target)

lower_bound:=値以上のものの最初のイテレータを返す
upper_bound:=値より大きいものの最初のイテレータを返す
例えば、multisetに556777とあって

最初の7が取得したいなら
mst.lower_bound(7);

最後の7が取得したいなら
auto itr = mst.lower_bound(7+1);
if(mst.begin()!= itr)itr--;
else cout << -1 << endl;





