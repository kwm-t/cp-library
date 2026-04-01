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
#endif

#endif // KWM_T_BASE_INTEGR_HPP