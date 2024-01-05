#include <utility>
struct HashPair {
	//注意 constがいる
	template<class T1, class T2>
	size_t operator()(const std::pair<T1, T2> &p) const {
		//first分をハッシュ化する
		auto hash1 = hash<T1>{}(p.first);
		//second分をハッシュ化する
		auto hash2 = hash<T2>{}(p.second);
		//重複しないようにハッシュ処理
		size_t seed = 0;
		seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};