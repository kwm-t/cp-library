#include <vector>
#include<iostream>
struct MyBitset {
	size_t n;
	std::vector<unsigned long long>bit;
	const size_t lg = 6;
	const size_t w = 1 << lg;
	const size_t mod_w = w - 1;
	const unsigned long long mask = (((1ULL << (w - 1)) - 1) << 1) + 1;
	MyBitset() :MyBitset(0) {}
	MyBitset(size_t n) :MyBitset(n, 0) {}
	MyBitset(size_t n, int v) :n(n) {
		bit.resize(ceil(n, w), 0);
	}
	int get(size_t pos) {
		size_t idx = pos >> lg;
		size_t j = pos & mod_w;
		return 1 & (bit[idx] >> j);
	}
	void set(size_t pos, int val = 1) {
		size_t idx = pos >> lg;
		size_t j = pos & mod_w;
		if (0 == val) {
			if (get(pos))bit[idx] ^= 1ULL << j;
		}
		else bit[idx] |= 1ULL << j;
	}
	void reset(size_t pos) {
		set(pos, 0);
	}
	int count() {
		int ret = 0;
		for (int i = 0; i < bit.size(); ++i)ret += popcount(bit[i]);
		return ret;
	}
	void debug() {
		for (int i = 0; i < bit.size(); ++i) std::cout << i << " " << bit[i] << std::endl;
	}
	mybitset& operator|=(const mybitset& rhs) {
		if (this->bit.size() < rhs.bit.size())this->bit.resize(rhs.bit.size(), 0);
		for (size_t i = 0; i < this->bit.size() && i < rhs.bit.size(); i++) {
			this->bit[i] |= rhs.bit[i];
		}
		return *this;
	}
	mybitset& operator&=(const mybitset& rhs) {
		if (this->bit.size() < rhs.bit.size())this->bit.resize(rhs.bit.size(), 0);
		for (size_t i = 0; i < this->bit.size() && i < rhs.bit.size(); i++) {
			this->bit[i] &= rhs.bit[i];
		}
		return *this;
	}
	mybitset& operator^=(const mybitset& rhs) {
		if (this->bit.size() < rhs.bit.size())this->bit.resize(rhs.bit.size(), 0);
		for (size_t i = 0; i < this->bit.size() && i < rhs.bit.size(); i++) {
			this->bit[i] ^= rhs.bit[i];
		}
		return *this;
	}
private:
	size_t ceil(size_t a, size_t b) { return (a + b - 1) / b; }
	int popcount(unsigned long long n) {
		// gcc提出用
		//return __builtin_popcountll(n / 2) + n % 2;
		return mypopcount(n / 2) + n % 2;
	}
	int mypopcount(long long n) {
		n = (n & 0x5555555555555555) + (n >> 1 & 0x5555555555555555);
		n = (n & 0x3333333333333333) + (n >> 2 & 0x3333333333333333);
		n = (n & 0x0f0f0f0f0f0f0f0f) + (n >> 4 & 0x0f0f0f0f0f0f0f0f);
		n = (n & 0x00ff00ff00ff00ff) + (n >> 8 & 0x00ff00ff00ff00ff);
		n = (n & 0x0000ffff0000ffff) + (n >> 16 & 0x0000ffff0000ffff);
		n = (n & 0x00000000ffffffff) + (n >> 32 & 0x00000000ffffffff);
		return n;
	}
};