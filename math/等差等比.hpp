using mint2 = atcoder::modint;
const int mod2 = mod - 1;
//mint2::set_mod(mod2);
// 等差数列*等比数列の和公式(n1項目までの和)
mint f(mint a, mint b, mint c, mint n1, mint2 n2) {//n2はmod-1
	if (1 == c.val())return (a *(n1 - 1)*n1 + b * n1) / 2;
	// Σ(ai+b)*c^i
	mint ret = 0;
	ret += a * ((n1 - 1) * pow_mod(c.val(), n2.val() + 1, mod)
		- n1 * pow_mod(c.val(), n2.val(), mod) + c);
	ret += b * (c - 1) * ((mint)pow_mod(c.val(), n2.val(), mod) - 1);
	ret /= (1 - c);
	ret /= (1 - c);
	return ret;
}