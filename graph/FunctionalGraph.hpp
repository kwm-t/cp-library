class FunctionalGraph {
public:
	FunctionalGraph(int size) :size(size) {
		edge.resize(size);
		edgeR.resize(size);
		vertexs.reserve(size);
		for (int i = 0; i < size; ++i)vertexs.push_back(Vertex(i));
	}
	struct Vertex {
		int index;
		int cycleIndex;
		int deep;
		Vertex(int index, int cycleIndex = -1, int deep = -1) :index(index), cycleIndex(cycleIndex), deep(deep) {}
	};
	void addEdge(int u, int v) {
		edge[u].push_back(v);
		edgeR[v].push_back(u);
	}
	void init() {
		int cycleIndex = 0;
		for (int i = 0; i < size; ++i) {
			if (-1 != vertexs[i].cycleIndex)continue;
			std::vector<int>roop;
			std::set<int>st;
			dfs0(i, roop, st);
			for (auto v : roop) {
				vertexs[v].cycleIndex = cycleIndex;
				vertexs[v].deep = 0;
			}
			cycleSize.push_back(roop.size());
			for (auto v : roop) {
				dfs1(v, cycleIndex, -1, 0);
			}
			cycleIndex++;
		}
	}
	std::vector<Vertex> getVertexs() {
		return vertexs;
	}
	std::vector<int> getCycleSize() {
		return cycleSize;
	}
	void dump() {
		for (auto v : vertexs) {
			std::cout << v.index << " " << v.deep << " " << v.cycleIndex << std::endl;
		}
		for (auto cs : cycleSize) {
			std::cout << cs << std::endl;
		}
	}
private:
	int size;
	std::vector<std::vector<int>>edge;
	std::vector<std::vector<int>>edgeR;
	std::vector<Vertex>vertexs;
	std::vector<int>cycleSize;
	bool dfs0(int v, std::vector<int> &roop, std::set<int>&st) {
		if (st.count(v)) {
			std::reverse(roop.begin(), roop.end());
			while (v != roop.back()) roop.pop_back();
			std::reverse(roop.begin(), roop.end());
			return true;
		}
		st.insert(v);
		roop.push_back(v);
		for (auto nv : edge[v]) {
			auto res = dfs0(nv,  roop, st);
			if (res)return true;
		}
		roop.pop_back();
		st.erase(v);
		return false;
	}
	void dfs1(int v, int cycleIndex, int p = -1, int deep = 0) {
		vertexs[v].cycleIndex = cycleIndex;
		vertexs[v].deep = deep;
		for (auto nv : edgeR[v]) {
			if (-1 != vertexs[nv].cycleIndex)continue;
			dfs1(nv, cycleIndex, v, deep + 1);
		}
	}
};