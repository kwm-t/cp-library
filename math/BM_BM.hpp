#ifndef BM_BM_HPP
#define BM_BM_HPP

#include <atcoder/all>
#include "Bostan_mori.hpp"
#include "BarlekampMassey.hpp"

// 線形漸化式の初めのd項からn項目を求めるための前処理 
// d次漸化式ならO(d^2+dlogdlogN).
template<class T>
Bostan_mori<T> BM_BM(std::vector<T> a) {
	auto q = BarlekampMassey(a);// O(d^2)
	a.resize(q.size() - 1);
	auto p = atcoder::convolution(a, q);// O(dlogd)
	p.resize(q.size() - 1);

	Bostan_mori<T> bm(p, q);
	return bm;// O(dlogdlogN)で取得
}

#endif