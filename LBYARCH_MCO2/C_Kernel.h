//C_Kernel.h
void c_domath(long long int n, float A, float* X, float* Y, float* Z) {
	for (long long i = 0; i < n; i++) {
		Z[i] = A * X[i] + Y[i];
	}
}

