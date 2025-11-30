// createTableで出力したものをコードに貼り付けcalcFactを呼ぶ
// BLOCK_SIZEは共通である必要がある
// BLOCK_SIZEを大きくするとコードが短くなり
// BLOCK_SIZEを小さくするとcalcFactが早くなる
const int BLOCK_SIZE = 1000000;
void createTable() {
#pragma warning(disable : 4996)
	//FILE *in = freopen("in/in.txt", "r", stdin);
	const std::string fileName = "out/fact" + std::to_string(mod) + ".txt";
	FILE *out = freopen(fileName.c_str(), "w", stdout);
	const int sz = (mod + BLOCK_SIZE - 1) / BLOCK_SIZE;
	std::cout << "std::vector<mint>fact{" << std::endl;
	mint val = 1;
	for (int i = 0; i <= sz * BLOCK_SIZE; ++i) {
		if (0 != i) {
			val *= i;
		}
		if (0 == i % BLOCK_SIZE) {
			if (sz * BLOCK_SIZE != i)std::cout << '\t' << val.val() << ',' << std::endl;
			else std::cout << '\t' << val.val() << std::endl;
		}
	}
	std::cout << "};" << std::endl;
}
std::vector<mint>fact{
};
mint calcFact(long long n) {
	if (n >= mod)return 0;
	mint ret = fact[n/ BLOCK_SIZE];
	for (int i = (n / BLOCK_SIZE) * BLOCK_SIZE + 1;i <= n;++i) {
		ret *= i;
	}
	return ret;
}
