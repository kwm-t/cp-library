multiset<int>mst
//一つだけ削除
mst.erase(mst.find(target));
//最小値
*mst.begin();
//最大値
*mst.rbegin();