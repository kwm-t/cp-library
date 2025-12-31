#include <cmath>
#define _USE_MATH_DEFINES
const double EPS = 1e-10;

struct Vector2D {
	// 平面ベクトル
	double x, y;

	Vector2D(double x, double y) : x(x), y(y) {}

	// ベクトルの和
	Vector2D operator+(const Vector2D p) { return Vector2D(x + p.x, y + p.y); }
	// ベクトルの差
	Vector2D operator-(const Vector2D p) { return Vector2D(x - p.x, y - p.y); }

	// ベクトルのスカラー倍
	Vector2D operator*(const double d) { return Vector2D(d * x, d * y); }
	Vector2D operator/(const double d) { return Vector2D(x / d, y / d); }

	// ベクトルの長さ
	double norm() { return std::sqrt(x * x + y * y); }

	// ベクトルの回転
	Vector2D rotate(double theta) {
		return Vector2D(x * cos(theta) - y * sin(theta), x * sin(theta) + y * cos(theta));
	}
};
struct Circle {
	// 中心
	Vector2D center;
	// 半径
	double radius;
	//コンストラクタ
	Circle(Vector2D center, double radius) : center(center), radius(radius) {}
	//含まれているか
	bool inside(Vector2D v) {
		double d = (center - v).norm();
		return d <= radius + EPS;
	}
};
//交点
std::vector<Vector2D> getIntersections(Circle C1, Circle C2) {
	double d = (C2.center - C1.center).norm();
	double rc = (C1.radius * C1.radius + d * d - C2.radius * C2.radius) / (2 * d);
	double rs = sqrt(C1.radius * C1.radius - rc * rc);
	if (d - EPS > C1.radius + C2.radius) return {};
	Vector2D e1 = (C2.center - C1.center) / d;
	Vector2D e2 = e1.rotate(M_PI / 2);
	Vector2D e3 = e1.rotate(-M_PI / 2);
	Vector2D A = C1.center + e1 * rc + e2 * rs;
	Vector2D B = C1.center + e1 * rc + e3 * rs;
	return { A, B };
}