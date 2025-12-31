#ifndef BINOM_MOD2_HPP
#define BINOM_MOD2_HPP
// Bionm(n,k)
int binom_mod2(long long n, long long k) {
	return (n&k) == k ? 1:0;
}

#endif