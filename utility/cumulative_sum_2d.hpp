#ifndef KWM_T_UTILITY_CUMULATIVE_SUM_2D_HPP
#define KWM_T_UTILITY_CUMULATIVE_SUM_2D_HPP

#include <vector>
#include <cassert>

namespace kwm_t::utility {

template <class T>
struct CumulativeSum2D {
private:
	std::vector<std::vector<T>> s;

public:
	CumulativeSum2D() = default;

	explicit CumulativeSum2D(const std::vector<std::vector<T>>& a) {
		build(a);
	}

	void build(const std::vector<std::vector<T>>& a) {
		int h = a.size();
		int w = h ? a[0].size() : 0;

		s.assign(h + 1, std::vector<T>(w + 1, T{}));

		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				s[i + 1][j + 1] = a[i][j];

		for (int i = 0; i <= h; i++)
			for (int j = 0; j < w; j++)
				s[i][j + 1] += s[i][j];

		for (int i = 0; i < h; i++)
			for (int j = 0; j <= w; j++)
				s[i + 1][j] += s[i][j];
	}

	int height() const { return (int)s.size() - 1; }
	int width() const { return s.empty() ? 0 : (int)s[0].size() - 1; }

	// [x0, x1) × [y0, y1)
	T sum(int x0, int y0, int x1, int y1) const {
		assert(0 <= x0 && x0 <= x1 && x1 <= height());
		assert(0 <= y0 && y0 <= y1 && y1 <= width());

		return s[x1][y1]
			- s[x1][y0]
			- s[x0][y1]
			+ s[x0][y0];
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_CUMULATIVE_SUM_2D_HPP