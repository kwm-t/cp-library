namespace beam_search {

	// ビームサーチの設定
	struct Config {
		int maxTurn;				// ビームサーチ深さ上限
		size_t beamWidth;			// ビーム幅
		size_t tourCapacity;		// オイラーツアーのキャパシティ
		uint32_t hashMapCapacity;	// HashMapのキャパシティ
	};

	// 連想配列
	// Keyにハッシュ関数を適用しない
	// open addressing with linear probing
	// unordered_mapより高速
	// nは格納する要素数よりも16倍ほど大きく確保をする
	template <class Key, class Value>
	struct HashMap {
	public:
		explicit HashMap(uint32_t n) {
			if (n % 2 == 0) {
				++n;
			}
			capacity = n;
			valid.resize(capacity, false);
			data.resize(capacity);
		}

		// 戻り値
		// - 存在するならtrue、存在しないならfalse
		// - index
		std::pair<bool, int> getIndex(Key key) const {
			Key i = key % capacity;
			while (valid[i]) {
				if (data[i].first == key) {
					return { true, i };
				}
				if (++i == capacity) {
					i = 0;
				}
			}
			return { false, i };
		}

		// 指定したindexにkeyとvalueを格納する
		void set(int i, Key key, Value value) {
			valid[i] = true;
			data[i] = { key, value };
		}

		// 指定したindexのvalueを返す
		Value get(int i) const {
			assert(valid[i]);
			return data[i].second;
		}

		void clear() {
			fill(valid.begin(), valid.end(), false);
		}

	private:
		uint32_t capacity;
		std::vector<bool> valid;
		std::vector<std::pair<Key, Value>> data;
	};

	using Hash = uint32_t; // TODO

	// 状態遷移を行うために必要な情報
	// メモリ使用量をできるだけ小さくしてください
	struct Action {
		// TODO

		Action() {
			// TODO
		}

		bool operator==(const Action& other) const {
			// TODO
		}
	};

	using Cost = int; // TODO

	// 状態のコストを評価するための構造体
	// メモリ使用量をできるだけ小さくしてください
	// Costが低いほど優秀
	struct Evaluator {
		// TODO

		Evaluator() {
			// TODO
		}

		// 低いほどよい
		Cost evaluate() const {
			// TODO
		}

		bool operator<(const Evaluator& other) const {
			return evaluate() < other.evaluate();
		}
	};

	// 展開するノードの候補を表す構造体
	struct Candidate {
		Action action;
		Evaluator evaluator;
		Hash hash;
		int parent;

		Candidate(Action action, Evaluator evaluator, Hash hash, int parent) :
			action(action),
			evaluator(evaluator),
			hash(hash),
			parent(parent) {}
	};

	// 削除可能な優先度付きキュー
	std::pair<Cost, int> op(std::pair<Cost, int> a, std::pair<Cost, int> b) {
		if (a.first >= b.first) return a;
		else return b;
	}
	std::pair<Cost, int> e() { return std::make_pair(std::numeric_limits<Cost>::min(), -1); }
	using MaxSegtree = atcoder::segtree <std::pair<Cost, int>, op, e>;

	// ノードの候補から実際に追加するものを選ぶクラス
	// ビーム幅の個数だけ、評価がよいものを選ぶ
	// ハッシュ値が一致したものについては、評価がよいほうのみを残す
	class Selector {
	public:
		explicit Selector(const Config& config) :
			hashMap(config.hashMapCapacity)
		{
			beamWidth = config.beamWidth;
			candidates.reserve(beamWidth);
			full = false;

			costs.resize(beamWidth);
			for (size_t i = 0; i < beamWidth; ++i) {
				costs[i] = { 0, i };
			}
		}

		// 候補を追加する
		// ターン数最小化型の問題で、candidateによって実行可能解が得られる場合にのみ finished = true とする
		// ビーム幅分の候補をCandidateを追加したときにsegment treeを構築する
		void push(const Candidate& candidate, bool finished) {
			if (finished) {
				finishedCandidates.emplace_back(candidate);
				return;
			}
			Cost cost = candidate.evaluator.evaluate();
			if (full && cost >= segTree.all_prod().first) {
				// 保持しているどの候補よりもコストが小さくないとき
				return;
			}
			auto[valid, i] = hashMap.getIndex(candidate.hash);

			if (valid) {
				int j = hashMap.get(i);
				if (candidate.hash == candidates[j].hash) {
					// ハッシュ値が等しいものが存在しているとき
					if (full) {
						// segment treeが構築されている場合
						if (cost < segTree.get(j).first) {
							candidates[j] = candidate;
							segTree.set(j, { cost, j });
						}
					}
					else {
						// segment treeが構築されていない場合
						if (cost < costs[j].first) {
							candidates[j] = candidate;
							costs[j].first = cost;
						}
					}
					return;
				}
			}
			if (full) {
				// segment treeが構築されている場合
				int j = segTree.all_prod().second;
				hashMap.set(i, candidate.hash, j);
				candidates[j] = candidate;
				segTree.set(j, { cost, j });
			}
			else {
				// segment treeが構築されていない場合
				int j = candidates.size();
				hashMap.set(i, candidate.hash, j);
				candidates.emplace_back(candidate);
				costs[j].first = cost;

				if (candidates.size() == beamWidth) {
					// 保持している候補がビーム幅分になったときにsegment treeを構築する
					full = true;
					segTree = MaxSegtree(costs);
				}
			}
		}

		// 選んだ候補を返す
		const std::vector<Candidate>& select() const {
			return candidates;
		}

		// 実行可能解が見つかったか
		bool haveFinished() const {
			return !finishedCandidates.empty();
		}

		// 実行可能解に到達するCandidateを返す
		std::vector<Candidate> getFinishedCandidates() const {
			return finishedCandidates;
		}

		// 最もよいCandidateを返す
		Candidate calculate_best_candidate() const {
			if (full) {
				size_t best = 0;
				for (size_t i = 0; i < beamWidth; ++i) {
					if (segTree.get(i).first < segTree.get(best).first) {
						best = i;
					}
				}
				return candidates[best];
			}
			else {
				size_t best = 0;
				for (size_t i = 0; i < candidates.size(); ++i) {
					if (costs[i].first < costs[best].first) {
						best = i;
					}
				}
				return candidates[best];
			}
		}

		void clear() {
			candidates.clear();
			hashMap.clear();
			full = false;
		}

	private:
		size_t beamWidth;
		std::vector<Candidate> candidates;
		HashMap<Hash, int> hashMap;
		bool full;
		std::vector<std::pair<Cost, int>> costs;
		MaxSegtree segTree;
		std::vector<Candidate> finishedCandidates;
	};

	// 深さ優先探索に沿って更新する情報をまとめたクラス
	class State {
	public:
		explicit State() {
			// TODO
		}

		// EvaluatorとHashの初期値を返す
		std::pair<Evaluator, Hash> makeInitialNode() {
			// TODO
		}

		// 次の状態候補を全てselectorに追加する
		// 引数
		//   evaluator : 今の評価器
		//   hash      : 今のハッシュ値
		//   parent    : 今のノードID（次のノードにとって親となる）
		void expand(const Evaluator& evaluator, Hash hash, int parent, Selector& selector) {
			// TODO
		}

		// actionを実行して次の状態に遷移する
		void moveForward(Action action) {
			// TODO
		}

		// actionを実行する前の状態に遷移する
		// 今の状態は、親からactionを実行して遷移した状態である
		void moveBackward(Action action) {
			// TODO
		}

	private:
		// TODO
	};

	// Euler Tourを管理するためのクラス
	class Tree {
	public:
		explicit Tree(const State& state, const Config& config) :
			state(state)
		{
			currTour.reserve(config.tourCapacity);
			nextTour.reserve(config.tourCapacity);
			leaves.reserve(config.beamWidth);
			buckets.assign(config.beamWidth, {});
		}

		// 状態を更新しながら深さ優先探索を行い、次のノードの候補を全てselectorに追加する
		void dfs(Selector& selector) {
			// 最初のターン
			if (currTour.empty()) {
				auto[evaluator, hash] = state.makeInitialNode();
				state.expand(evaluator, hash, 0, selector);
				return;
			}

			for (auto &[leafIndex, action] : currTour) {
				if (leafIndex >= 0) {
					// 葉
					state.moveForward(action);
					auto&[evaluator, hash] = leaves[leafIndex];
					state.expand(evaluator, hash, leafIndex, selector);
					state.moveBackward(action);
				}
				else if (leafIndex == -1) {
					// 前進辺
					state.moveForward(action);
				}
				else {
					// 後退辺
					state.moveBackward(action);
				}
			}
		}

		// 木を更新する
		void update(const std::vector<Candidate>& candidates) {
			leaves.clear();

			if (currTour.empty()) {
				// 最初のターン
				for (const Candidate& candidate : candidates) {
					currTour.push_back({ (int)leaves.size(), candidate.action });
					leaves.push_back({ candidate.evaluator, candidate.hash });
				}
				return;
			}

			for (const Candidate& candidate : candidates) {
				buckets[candidate.parent].push_back({ candidate.action, candidate.evaluator, candidate.hash });
			}

			auto it = currTour.begin();

			// 一本道を反復しないようにする
			while (it->first == -1 && it->second == currTour.back().second) {
				Action action = (it++)->second;
				state.moveForward(action);
				directRoad.push_back(action);
				currTour.pop_back();
			}

			// 葉の追加や不要な辺の削除をする
			while (it != currTour.end()) {
				auto[leafIndex, action] = *(it++);
				if (leafIndex >= 0) {
					// 葉
					// 追加する葉がない
					if (buckets[leafIndex].empty()) {
						continue;
					}
					// 葉の追加
					nextTour.push_back({ -1, action });
					for (auto[newAction, evaluator, hash] : buckets[leafIndex]) {
						int newLeafIndex = leaves.size();
						nextTour.push_back({ newLeafIndex, newAction });
						leaves.push_back({ evaluator, hash });
					}
					buckets[leafIndex].clear();
					nextTour.push_back({ -2, action });
				}
				else if (leafIndex == -1) {
					// 前進辺
					nextTour.push_back({ -1, action });
				}
				else {
					// 後退辺
					auto[oldLeafIndex, oldAction] = nextTour.back();
					if (oldLeafIndex == -1) {
						// 不要な前進辺の削除
						nextTour.pop_back();
					}
					else {
						// 後退辺の追加
						nextTour.push_back({ -2, action });
					}
				}
			}
			std::swap(currTour, nextTour);
			nextTour.clear();
		}

		// 根からのパスを取得する
		std::vector<Action> calculatePath(int parent, int turn) const {
			std::vector<Action> ret = directRoad;
			ret.reserve(turn);
			for (auto[leafIndex, action] : currTour) {
				if (leafIndex >= 0) {
					// 葉
					if (leafIndex == parent) {
						ret.push_back(action);
						return ret;
					}
				}
				else if (leafIndex == -1) {
					// 前進辺
					ret.push_back(action);
				}
				else {
					// 後退辺
					ret.pop_back();
				}
			}
		}

	private:
		State state;
		std::vector<std::pair<int, Action>> currTour;
		std::vector<std::pair<int, Action>> nextTour;
		std::vector<std::pair<Evaluator, Hash>> leaves;
		std::vector<std::vector<std::tuple<Action, Evaluator, Hash>>> buckets;
		std::vector<Action> directRoad;// 固定経路
	};

	// ビームサーチを行う関数
	std::vector<Action> beamSearch(const Config& config, const State& state) {
		Tree tree(state, config);

		// 新しいノード候補の集合
		Selector selector(config);

		for (int turn = 0; turn < config.maxTurn; ++turn) {
			// Euler Tourでselectorに候補を追加する
			tree.dfs(selector);

			if (selector.haveFinished()) {
				// ターン数最小化型の問題で実行可能解が見つかったとき
				Candidate candidate = selector.getFinishedCandidates()[0];
				std::vector<Action> ret = tree.calculatePath(candidate.parent, turn + 1);
				ret.push_back(candidate.action);
				return ret;
			}

			assert(!selector.select().empty());

			if (turn == config.maxTurn - 1) {
				// ターン数固定型の問題で全ターンが終了したとき
				Candidate bestCandidate = selector.calculate_best_candidate();
				std::vector<Action> ret = tree.calculatePath(bestCandidate.parent, turn + 1);
				ret.push_back(bestCandidate.action);
				return ret;
			}

			// 木を更新する
			tree.update(selector.select());

			selector.clear();
		}

		//unreachable();
	}
} // namespace beam_search