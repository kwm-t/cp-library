#ifdef _MSC_VER
#define __builtin_popcount (int)__popcnt
#define __builtin_popcountll (int)__popcnt64
inline int __builtin_ctz(unsigned int n) { unsigned long i; _BitScanForward(&i, n); return i; }
inline int __builtin_ctzll(unsigned long long n) { unsigned long i; _BitScanForward64(&i, n); return i; }
inline int __builtin_clz(unsigned int n) { unsigned long i; _BitScanReverse(&i, n); return i; }
inline int __builtin_clzll(unsigned long long n) { unsigned long i; _BitScanReverse64(&i, n); return i; }
#endif