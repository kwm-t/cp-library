#pragma once
// nに応じて、その桁数のレピュニット数を返す
// ex 1,11,111,1111
// modPにおいて,Pが9の逆元を持つなら
// pow_mod(10,n,mod)/9とすれば十分だが逆元を持たないときに使う
template<typename T>
T repunit(long long n) {
	T ret = 0;
	T rpw10 = 1;
	T tmp = 1;
	T pw10 = 10;
	while (n) {
		if (n % 2) {
			ret += rpw10 * tmp;
			rpw10 *= pw10;
		}
		tmp += tmp * pw10;
		pw10 *= pw10;
		n /= 2;
	}
	return ret;
}