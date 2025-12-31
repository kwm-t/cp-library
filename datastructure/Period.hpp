template <typename Index, typename Data, Index(*next_index)(Index), Data(*get_data)(Index), Data(*op)(Data, Data), Data(*e)(), Data(*power)(Data, long long)>
struct Period {
private:
	std::vector<Data>dat, preData, cycData;
	int prefixSize, cycleSize;
	Data front, back;
public:
	Period(Index start) {
		Index a = start, b = start;
		do {// フロイドの循環検出法
			dat.emplace_back(get_data(a));
			a = next_index(a);
			b = next_index(next_index(b));
		} while (a != b);
		cycleSize = (int)dat.size();
		Index c = start;
		while (a != c) {
			dat.emplace_back(get_data(a));
			a = next_index(a);
			c = next_index(c);
		}
		prefixSize = (int)dat.size() - cycleSize;
		preData.resize((int)dat.size() + 1);
		cycData.resize((int)dat.size() + 1);
		preData[0] = e(), cycData[prefixSize] = e();
		for (int i = 1; i < (int)preData.size(); i++) {
			preData[i] = op(preData[i - 1], dat[i - 1]);
		}
		for (int i = prefixSize + 1; i < (int)cycData.size(); i++) {
			cycData[i] = op(cycData[i - 1], dat[i - 1]);
		}
		front = preData[prefixSize];
		back = cycData[prefixSize + cycleSize];
	}
	Data query(long long k) {
		if (k <= (int)dat.size()) return preData[k];
		Data remain = cycData[prefixSize + (k - prefixSize) % cycleSize];
		return op(op(front, power(back, (k - prefixSize) / cycleSize)), remain);
	}
};
/*
Readme
using Index = long long;
using Data = long long;
int M;
Index next_index(Index x) { return x * x % M; }
Data get_data(Index x) { return x; }
Data op(Data a, Data b) { return a + b; }
Data e() { return 0; }
Data power(Data a, long long n) { return n * a; }
Period<Index, Data, next_index, get_data, op, e, power> pe(X);*/