#include<array>
using point = std::array<long long, 3>;
long long crossproduct(point p, point q, point x) {
	//pqからみてxが反時計回りなら正,線分上なら0,時計回りなら負
	long long x1 = q[0] - p[0];
	long long y1 = q[1] - p[1];
	long long x2 = x[0] - p[0];
	long long y2 = x[1] - p[1];
	return x1 * y2 - y1 * x2;
}
long long innerproduct(point p, point q, point r) {
	//内積
	long long x1 = q[0] - p[0];
	long long y1 = q[1] - p[1];
	long long x2 = r[0] - p[0];
	long long y2 = r[1] - p[1];
	return x1 * x2 + y1 * y2;
}
int check(point p, point q, point r) {
	//rが、pからqを見て左側にあれば1、線分pq上にあれば0、右側にあれば-1
	//線分の延長上にあれば-2
	long long c = crossproduct(p, q, r);
	if (c)return c > 0 ? 1 : -1;
	long long i = innerproduct(p, q, r);
	long long d = innerproduct(q, p, r);
	return (0 <= i && 0 <= d) ? 0 : -2;
}