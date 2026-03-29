#ifndef KWM_T_MATH_XOR_LINEAR_BASIS_HPP
#define KWM_T_MATH_XOR_LINEAR_BASIS_HPP

#include <vector>
#include <algorithm>

namespace kwm_t::math {

    // XOR linear basis (minimum form, noshi91 style)
    template <class T>
    std::vector<T> xor_linear_basis(std::vector<T> a) {
        std::vector<T> basis;

        for (size_t i = 0; i < a.size(); ++i) {
            // reduce a[i]
            for (const auto& b : basis) {
                a[i] = std::min(a[i], a[i] ^ b);
            }
            if (a[i] == 0) continue;

            // reduce existing basis
            for (auto& b : basis) {
                b = std::min(b, b ^ a[i]);
            }

            basis.push_back(a[i]);
        }

        std::sort(basis.begin(), basis.end());
        return basis;
    }

    // XOR linear basis with original indices
    template <class T>
    std::vector<std::pair<T, int>> xor_linear_basis_with_index(const std::vector<T>& a) {
        std::vector<std::pair<T, int>> basis;
        basis.reserve(a.size());

        for (int i = 0; i < (int)a.size(); ++i) {
            T x = a[i];

            // reduce x
            for (const auto& [b, _] : basis) {
                x = std::min(x, x ^ b);
            }
            if (x == 0) continue;

            // reduce existing basis
            for (auto& [b, _] : basis) {
                b = std::min(b, b ^ x);
            }

            basis.emplace_back(x, i);
        }

        std::sort(basis.begin(), basis.end());
        return basis;
    }

} // namespace kwm_t::math

#endif