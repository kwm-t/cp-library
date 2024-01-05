#include <bits/stdc++.h>
//#include <atcoder/all>
//using namespace std;
//using namespace atcoder;
//using mint = modint998244353;
//const int mod = 998244353;
//using mint = modint1000000007;
//const int mod = 1000000007;
//const int INF = 1e9;
//const long long LINF = 1e18;
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define rep2(i,l,r)for(int i=(l);i<(r);++i)
#define rrep(i, n) for (int i = (n-1); i >= 0; --i)
#define rrep2(i,l,r)for(int i=(r-1);i>=(l);--i)
#define all(x) (x).begin(),(x).end()
#define allR(x) (x).rbegin(),(x).rend()
std::random_device rnd;
std::mt19937 engine(rnd());
std::uniform_real_distribution<> randR(0.0, 1.0);
struct Operation {
	int move_dir;
	Operation(int move_dir) : move_dir(move_dir) {}
};
std::vector<Operation> valid_operations;
void SetOperations() {
	for (int dir = 0; dir < 4; dir++) {
		valid_operations.emplace_back(dir);
	}
}
// 問題固有
struct State {
	int now_turn;
	int coins;
	int penalty;
	unsigned long long zobrist_hash;//盤面のハッシュ値
	std::vector<std::pair<int, int>>player;
	std::vector<std::bitset<2500>>visited;

	State();
	int score() const;
	unsigned long long hash() const;
	std::pair<int, unsigned long long> try_move(const Operation & op)const;
	void apply_move(const Operation & op);
};
State::State() {
	// 初期化(初期盤面のハッシュ値の計算、プレイヤーの位置情報の取得など)
}
int State::score() const {
	// 評価関数を設定する
}
unsigned long long State::hash() const {
	// ハッシュ値、今回はzobrist hash
}
std::pair<int, unsigned long long> State::try_move(const Operation& op) const {
	// 一手進めた場合のスコアとハッシュ値を返す、ただし状態を更新はしない
}
void State::apply_move(const Operation& op) {
	// ここにターンを進める処理を書く
	// (スコアとハッシュ値の計算、プレイヤーや訪問済み情報の更新など)
}

// 永続Stack
struct History {
	Operation op;
	std::shared_ptr<History>parent;
	History(const Operation& op, std::shared_ptr<History>& parent) : op(op), parent(parent) {}
};
struct Stack {
	std::shared_ptr<History>head;
	Operation top();
	Stack push(const Operation &op);
	Stack pop();
};
Operation Stack::top() { return head->op; }
Stack Stack::push(const Operation& op) { return Stack({ std::make_shared<History>(op, head) }); }
Stack Stack::pop() { return Stack({ head->parent }); }

// ノード情報：基本的には普遍
struct  Node {
	State state;
	Stack move_history;

	Node(State& state);
	int get_score() const;
	unsigned long long get_hash() const;
	void advance(const Operation& op);
	std::pair<int, unsigned long long>calculate(const Operation& op) const;
};
Node::Node(State& state) : state(state) {}
int Node::get_score() const { return state.score(); }
unsigned long long Node::get_hash() const { return state.hash(); }
void Node::advance(const Operation& op) { state.apply_move(op); }
std::pair<int, unsigned long long> Node::calculate(const Operation& op) const { return state.try_move(op); }


// 仮ノード
struct TemporaryNode {
	int score;
	unsigned long long hash;
	int node_index;
	Operation op;
	double rand;//同一スコア時に利用

	TemporaryNode(int score, unsigned long long hash, int node_index, Operation& op);
};
TemporaryNode::TemporaryNode(int score, unsigned long long hash, int node_index, Operation& op) :score(score), hash(hash), node_index(node_index), op(op) { rand = randR(engine); }

// ビームサーチ改良版
Node BeamSearch(State& init_state, const int max_depth, const int beam_width) {
	std::vector<Node>nodes, next_nodes;
	nodes.emplace_back(init_state);
	nodes.back().move_history = Stack{ nullptr };

	std::vector<TemporaryNode>temp_nodes;// スコア比較用の仮ノードを保管
	std::unordered_set<unsigned long long> fields; // 重複除去用

	for (int turn = 0; turn < max_depth; ++turn) {
		temp_nodes.clear();
		fields.clear();

		for (int i = 0; i < (int)nodes.size(); ++i) {
			for (auto &op : valid_operations) {
				auto[next_score, next_hash] = nodes[i].calculate(op);
				temp_nodes.emplace_back(next_score, next_hash, i, op);

				if (fields.count(temp_nodes.back().hash)) temp_nodes.pop_back();
				else fields.insert(temp_nodes.back().hash);
			}
		}
		int node_size = temp_nodes.size();
		if (node_size > beam_width) {
			nth_element(temp_nodes.begin(), temp_nodes.begin() + beam_width, temp_nodes.end(),
				[](TemporaryNode& lh, TemporaryNode& rh) {
					if (lh.score == rh.score) {
						return lh.rand > rh.rand;
					}
					return lh.score > rh.score;
				});
		}

		for (int i = 0; i < std::min(beam_width, node_size); ++i) {
			int index = temp_nodes[i].node_index;
			next_nodes.emplace_back(nodes[index]);
			next_nodes.back().advance(temp_nodes[i].op);
			next_nodes.back().move_history = nodes[index].move_history.push(temp_nodes[i].op);
		}
		swap(nodes, next_nodes);
		next_nodes.clear();
	}
	int arg_best = -1, best_score = 0;
	for (int i = 0; i < (int)nodes.size(); i++) {
		if (nodes[i].get_score() > best_score) {
			arg_best = i;
			best_score = nodes[i].get_score();
		}
	}
	return nodes[arg_best];
}
int main() {
	return 0;
}