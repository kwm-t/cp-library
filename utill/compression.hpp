//‚±‚ê‚ª‘‚¢B
auto tmp = a;
std::sort(tmp.begin(), tmp.end());
tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
for (int i = 0; i < n; ++i) a[i] = lower_bound(tmp.begin(), tmp.end(), a[i]) - tmp.begin();