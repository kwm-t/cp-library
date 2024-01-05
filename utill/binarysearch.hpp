double ok = 0.0;
double ng = 1000.0;
for (int i = 0; i < 100; ++i) {//tle‚·‚é‚È‚ç’²®‚·‚é
	double mid = (ok + ng) / 2;
	if (f(mid)) ok = mid;
	else ng = mid;
}
printf("%.10f\n", ok);

long long ng = -1;
long long ok = LINF + 1;
while (1 < abs(ok - ng)) {
	long long mid = (ng + ok) / 2;
	auto check = [&]() {
		return false;
	};
	if (check()) ok = mid;
	else ng = mid;
}
cout << ok << endl;