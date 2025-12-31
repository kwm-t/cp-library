typedef std::complex<long long> C;
// 外積
long long cross(C a, C b) { return a.real() * b.imag() - a.imag() * b.real(); }
// 内積
long long dot(C a, C b) { return a.real() * b.real() + a.imag() * b.imag(); }
struct Segment {
	C a, b;
	Segment(C a = {}, C b = {}) :a(a), b(b) {}
	bool isOn(C c) {
		if (c == a) return true;
		if (c == b) return true;
		C v0 = a - c;
		C v1 = b - c;
		if (cross(v0, v1) != 0) return false;
		return dot(v0, v1) < 0;
	}
	bool isParallel(Segment seg) {
		C v = a - b;
		C w = seg.a - seg.b;
		return cross(v, w) == 0;
	}
	bool isIntersect(Segment seg) {
		if (isParallel(seg)) {
			if (isOn(seg.a)) return true;
			if (isOn(seg.b)) return true;
			if (seg.isOn(a)) return true;
			if (seg.isOn(b)) return true;
			return false;
		}
		auto v0 = b - a;
		auto v1 = seg.a - a;
		auto v2 = seg.b - a;
		if (cross(v0, v1) * cross(v0, v2) > 0) return false;
		
		v0 = seg.b - seg.a;
		v1 = a - seg.a;
		v2 = b - seg.a;
		if (cross(v0, v1) * cross(v0, v2) > 0) return false;
		return true;
	}
};