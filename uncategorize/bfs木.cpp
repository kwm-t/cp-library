	vector<int>idx(n, -1);//org->trans
	vector<int>rev(n, -1);//trans->org
	vector<int>par(n, -1);//trans
	vector<P>child(n, { INF,-INF });//trans
	vector<P>child2(n, { INF,-INF });//trans
	{
		queue<int>q;// trans
		int cnt = 0;
		q.push(cnt);
		idx[0] = cnt;
		rev[cnt] = 0;
		while (!q.empty()) {
			auto top = q.front();
			q.pop();
			rep(i, to[rev[top]].size()) {
				int org = to[rev[top]][i];
				int trans = idx[org];
				if (-1 != trans)continue;
				cnt++;
				trans = cnt;
				idx[org] = trans;
				rev[trans] = org;
				q.push(trans);
				chmin(child[top].first, trans);
				chmax(child[top].second, trans);
				par[trans] = top;
				int p = par[top];
				if (-1 != p) {
					chmin(child2[p].first, cnt);
					chmax(child2[p].second, cnt);
				}
			}
		}
		/*rep(i, n) {
			cout << i << " " << idx[i] << endl;
			cout << child[idx[i]].first << " " << child[idx[i]].second << endl;
			cout << child2[idx[i]].first << " " << child2[idx[i]].second << endl;
			cout << endl;
		}*/
	}