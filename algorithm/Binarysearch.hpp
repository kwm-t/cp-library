template <class T, class F>
T binarySearch(T ok, T ng, const F &f) {
	while (abs(ok - ng) > 1) {
		T mid = (ok + ng) / 2;
		(f(mid) ? ok : ng) = mid;
	}
	return ok;
}
template <class T, class F>
T binarySearchDouble(T ok, T ng, const F &f, int counter = 100) {
	while (counter--) {
		T mid = (ok + ng) / 2;
		(f(mid) ? ok : ng) = mid;
	}
	return ok;
}
/*
auto f = [&](const long long x)->bool {
	return true;
};*/