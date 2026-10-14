/**
 * @file integer.hpp
 * @brief 整数型に関する補助機能
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/base/integer.hpp
 */
#ifndef KWM_T_BASE_INTEGR_HPP
#define KWM_T_BASE_INTEGR_HPP

#include <vector>

/**
 * @brief ローカル環境用(MSC)int128
 */
#ifdef _MSC_VER
using int128 = long long;
#else
using int128 = __int128;
std::ostream& operator<<(std::ostream& dest, int128 value) {
	if (value == 0) {
		return dest << 0;
	}
	if (value < 0) {
		dest << '-';
		value = -value;
	}
	char buffer[128];
	char* p = end(buffer);
	while (value > 0) {
		*--p = '0' + (value % 10);
		value /= 10;
	}
	return dest.write(p, end(buffer) - p);
}
#endif

#endif // KWM_T_BASE_INTEGR_HPP