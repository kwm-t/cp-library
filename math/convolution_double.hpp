#include <vector>
#include <complex>

const double pi = acos(-1);

std::vector<std::complex<double>> fft(const std::vector<std::complex<double>> &a, double inv) {
	// inv = 1 ならば普通の FFT，inv = -1 ならば IFFT になるようにする．
	int n = (int)a.size();
	if (1 == n)return a;

	std::vector<std::complex<double>> ret;

	std::vector<std::complex<double>> even;
	std::vector<std::complex<double>> odd;
	for (int i = 0; i < n; ++i) {
		if (0 == i % 2) even.push_back(a[i]);
		else odd.push_back(a[i]);
	}

	auto d_even = fft(even, inv);
	auto d_odd = fft(odd, inv);

	const std::complex<double> zeta(cos(2 * pi * inv / (double)n), sin(2 * pi * inv / (double)n));

	std::complex<double> now = 1;
	for (int i = 0; i < n; i++) {
		ret.push_back(d_even[i % (n / 2)] + now * d_odd[i % (n / 2)]);
		now *= zeta;
	}
	return ret;
}

std::vector<double> convolution(std::vector<std::complex<double>> a, std::vector<std::complex<double>> b) {
	int sizeA = (int)a.size();
	int sizeB = (int)b.size();
	int sizeC = sizeA + sizeB - 1;

	int n = 1;
	while (n <= sizeC) n *= 2;

	a.resize(n);
	b.resize(n);

	auto da = fft(a, 1);
	auto db = fft(b, 1);

	std::vector<std::complex<double>> dc(n);
	for (int i = 0; i < n; ++i) dc[i] = da[i] * db[i];

	auto c = fft(dc, -1);

	// IFFT の後は最後に n で割る必要がある
	std::vector<double> ret(n);
	for (int i = 0; i < n; ++i) ret[i] = c[i].real() / (double)n;
	return ret;
}