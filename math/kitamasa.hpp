class Kitamasa {
	/*
		https://suikaba.hatenablog.com/entry/2017/08/24/174759
		ak = d0*a0 + d1*a1 + d2*a2 + ............d(k-1)*a(k-1)


		a(n)   = x0*a0 + x1*a1 + .......... x(k-1)*a(k-1)
		a(n+1) = x0*a1 + x1*a2 + .......... x(k-1)*ak
			   = (0 + x(k-1)*d0)a0 + (x0 + x(k-1)*d1)a1 ......(x(k-2) + x(k-1)*d(k-1)a(k-1)

		としa(n+k-1)までのk項をO(k^2)で列挙できる

		a(2n) = x0*an + x1*a(n+1) + .......... x(k-1)*a(n+k-1)
		列挙できているしているお陰でa0,,,,,a(k-1)の線形和に変形できる

		a(n)からa(n+1)とa(2n)が遷移できるのでO(logN)で目的のものが取得できる

		らしい
	*/
public:
	// d: coefficient
	Kitamasa(std::vector<mint> const& d_)
		: k(d_.size()),
		d(d_)
	{}

	void inc(std::vector<mint>& v) {
		std::rotate(v.begin(), v.begin() + (k - 1), v.begin() + k);
		mint l = v[0];
		v[0] = 0;
		for (int i = 0; i < k; ++i) {
			v[i] += d[i] * l;
		}
	}
	void dbl(std::vector<mint>& v) {
		auto buf = v;
		std::vector<mint> res(k);
		for (int i = 0; i < k; ++i) {
			for (int j = 0; j < k; ++j) {
				res[j] += buf[j] * v[i];
			}
			inc(buf);
		}
		v = std::move(res);
	}

	// calc a_n
	mint calc(long long n) {
		std::vector<mint> res(k);
		res[0] = 1;
		int j = 63;
		while (!(n & (1 << j))) {
			--j;
		}
		for (int i = j + 1; i >= 0; --i) {
			dbl(res);
			if (n & (1LL << i)) {
				inc(res);
			}
		}

		mint ans = 0;
		for (int i = 0; i < k; ++i) {
			ans += res[i] * d[i];
		}
		return ans;
	}

private:
	int k;
	std::vector<mint> d;
};