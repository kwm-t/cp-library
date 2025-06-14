#ifndef BM_BM_HPP
#define BM_BM_HPP

#include <atcoder/all>
#include "Bostan_mori.hpp"
#include "BarlekampMassey.hpp"

// üŒ`‘Q‰»®‚Ì‰‚ß‚Ìd€‚©‚çn€–Ú‚ğ‹‚ß‚é‚½‚ß‚Ì‘Oˆ— 
// dŸ‘Q‰»®‚È‚çO(d^2+dlogdlogN).
template<class T>
Bostan_mori<T> BM_BM(std::vector<T> a) {
	auto q = BarlekampMassey(a);// O(d^2)
	a.resize(q.size() - 1);
	auto p = atcoder::convolution(a, q);// O(dlogd)
	p.resize(q.size() - 1);

	Bostan_mori<T> bm(p, q);
	return bm;// O(dlogdlogN)‚Åæ“¾
}

#endif