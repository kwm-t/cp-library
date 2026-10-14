/**
 * @file imos_3d.hpp
 * @brief 三次元 Imos 法
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/utility/imos_3d.hpp
 */
#ifndef KWM_T_UTILITY_IMOS_3D_HPP
#define KWM_T_UTILITY_IMOS_3D_HPP

#include <vector>
#include <cassert>

namespace kwm_t::utility {

/**
 * @brief 3次元いもす法（3D Difference Array / 3D Imos）
 *
 * 典型用途:
 *   - 直方体領域への一括加算
 *   - 3次元空間への大量区間更新
 *   - 重なり数計算
 *
 * 計算量:
 *   - add: O(1)
 *   - build: O(HWD)
 *   - get: O(1)
 *
 * @tparam T
 *   加算・減算が定義されている型
 *
 * 制約 / 注意:
 *   - add(x0, y0, z0, x1, y1, z1, val) は
 *       半開区間
 *       [x0, x1) × [y0, y1) × [z0, z1)
 *     に val を加算する
 *   - 0-indexed を前提とする
 *   - build() 後に実際の値が得られる
 *
 * 使用例:
 *   // 10×10×10 空間
 *   kwm_t::utility::Imos3D<int> imos(10, 10, 10);
 *
 *   // 直方体領域に +5
 *   imos.add(1, 2, 3, 5, 6, 7, 5);
 *
 *   imos.build();
 *
 *   int v = imos.get(2, 3, 4);
 *
 * verified:
 *   - 
 */
template <class T>
struct Imos3D {
private:
	std::vector<std::vector<std::vector<T>>> d;
	bool built = false;

public:
	Imos3D() = default;

	Imos3D(int x, int y, int z) {
		init(x, y, z);
	}

	void init(int x, int y, int z) {
		assert(x >= 0 && y >= 0 && z >= 0);

		d.assign(
			x + 1,
			std::vector<std::vector<T>>(
				y + 1,
				std::vector<T>(z + 1, T{})
				)
		);

		built = false;
	}

	int size_x() const {
		return (int)d.size() - 1;
	}

	int size_y() const {
		return d.empty() ? 0 : (int)d[0].size() - 1;
	}

	int size_z() const {
		return d.empty() || d[0].empty()
			? 0
			: (int)d[0][0].size() - 1;
	}

	/**
	 * @brief 直方体領域
	 * [x0,x1) × [y0,y1) × [z0,z1)
	 * に val を加算
	 */
	void add(
		int x0, int y0, int z0,
		int x1, int y1, int z1,
		T val
	) {
		assert(!built);

		assert(0 <= x0 && x0 <= x1 && x1 <= size_x());
		assert(0 <= y0 && y0 <= y1 && y1 <= size_y());
		assert(0 <= z0 && z0 <= z1 && z1 <= size_z());

		d[x0][y0][z0] += val;
		d[x1][y0][z0] -= val;
		d[x0][y1][z0] -= val;
		d[x0][y0][z1] -= val;

		d[x1][y1][z0] += val;
		d[x1][y0][z1] += val;
		d[x0][y1][z1] += val;

		d[x1][y1][z1] -= val;
	}

	/**
	 * @brief 累積和を取って復元
	 */
	void build() {
		assert(!built);

		int X = size_x();
		int Y = size_y();
		int Z = size_z();

		// z方向
		for (int x = 0; x <= X; x++)
			for (int y = 0; y <= Y; y++)
				for (int z = 0; z < Z; z++)
					d[x][y][z + 1] += d[x][y][z];

		// y方向
		for (int x = 0; x <= X; x++)
			for (int y = 0; y < Y; y++)
				for (int z = 0; z <= Z; z++)
					d[x][y + 1][z] += d[x][y][z];

		// x方向
		for (int x = 0; x < X; x++)
			for (int y = 0; y <= Y; y++)
				for (int z = 0; z <= Z; z++)
					d[x + 1][y][z] += d[x][y][z];

		built = true;
	}

	/**
	 * @brief build 後の値を取得
	 */
	T get(int x, int y, int z) const {
		assert(built);

		assert(0 <= x && x < size_x());
		assert(0 <= y && y < size_y());
		assert(0 <= z && z < size_z());

		return d[x][y][z];
	}

	/**
	 * @brief build 後の配列を取得
	 */
	const std::vector<std::vector<std::vector<T>>>& data() const {
		assert(built);
		return d;
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_IMOS_3D_HPP