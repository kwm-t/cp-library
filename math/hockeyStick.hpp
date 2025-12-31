// Binom(k,k) + Binom(k+1,k)......Binom(n,k) = Binom(n+1,k+1)
mint hockeyStick(int n, int k) {
	return c(n + 1, k + 1);
}

// return :Binom(n,k) + Binom(n+1,k)......Binom(m,k)
mint hockeyStick(int n, int m, int k) {
	return hockeyStick(m, k) - hockeyStick(n - 1, k);
}