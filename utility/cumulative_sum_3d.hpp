#ifndef UTILITY_CUMULATIVE_SUM_3D_HPP
#define UTILITY_CUMULATIVE_SUM_3D_HPP

#include <vector>
#include <cassert>

namespace utility {

	template <class T>
	struct CumulativeSum3D {
	private:
		std::vector<std::vector<std::vector<T>>> s;

	public:
		CumulativeSum3D() = default;

		explicit CumulativeSum3D(const std::vector<std::vector<std::vector<T>>>& a) {
			build(a);
		}

		void build(const std::vector<std::vector<std::vector<T>>>& a) {
			int h = a.size();
			int w = h ? a[0].size() : 0;
			int d = (h && w) ? a[0][0].size() : 0;

			s.assign(h + 1,
				std::vector<std::vector<T>>(w + 1,
					std::vector<T>(d + 1, T{})));

			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					for (int k = 0; k < d; k++)
						s[i + 1][j + 1][k + 1] = a[i][j][k];

			for (int i = 0; i <= h; i++)
				for (int j = 0; j <= w; j++)
					for (int k = 0; k < d; k++)
						s[i][j][k + 1] += s[i][j][k];

			for (int i = 0; i <= h; i++)
				for (int j = 0; j < w; j++)
					for (int k = 0; k <= d; k++)
						s[i][j + 1][k] += s[i][j][k];

			for (int i = 0; i < h; i++)
				for (int j = 0; j <= w; j++)
					for (int k = 0; k <= d; k++)
						s[i + 1][j][k] += s[i][j][k];
		}

		// [x0,x1) × [y0,y1) × [z0,z1)
		T sum(int x0, int y0, int z0, int x1, int y1, int z1) const {
			return  s[x1][y1][z1]
				- s[x1][y1][z0]
				- s[x1][y0][z1]
				- s[x0][y1][z1]
				+ s[x0][y0][z1]
				+ s[x0][y1][z0]
				+ s[x1][y0][z0]
				- s[x0][y0][z0];
		}
	};

} // namespace utility

#endif // UTILITY_CUMULATIVE_SUM_3D_HPP