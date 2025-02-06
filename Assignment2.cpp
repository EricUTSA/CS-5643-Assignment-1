#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>


void Mult(const int& dim, int** const& a, int** const& b, int** const& c, const int& row, const int& num_rows) {
	for (int r = row; r < row + num_rows; ++r) {
		for (int i = 0; i < dim; ++i) {
			int sum = 0;
			for (int j = 0; j < dim; ++j) {
				sum += a[r][j] * b[j][i];
			}
			c[r][i];
		}
	}
}

void MatMultiply(const int& num_processors, const int& dim, int** const& a, int** const& b, int** const& c) {
	std::vector<std::thread> pool;

	const int chunk_size = dim / num_processors;
	int chunk = 0;

	for (int proc = 0; proc < num_processors - 1; ++proc) {
		pool.push_back(std::thread(Mult, dim, a, b, c, chunk, chunk_size));
		chunk += chunk_size;
	}

	pool.push_back(std::thread(Mult, dim, a, b, c, chunk, dim - chunk));

	std::for_each(pool.begin(), pool.end(), std::mem_fn(&std::thread::join));
}


int main() {
	constexpr int dim = 16;

	int** X = new int*[dim];
	int** Y = new int*[dim];
	int** Z = new int*[dim];

	for (int i = 0; i < dim; ++i) {
		X[i] = new int[dim];
		Y[i] = new int[dim];
		Z[i] = new int[dim];
		for (int j = 0; j < dim; ++j) {
			X[i][j] = i + j;
			Y[i][j] = i - j;
			Z[i][j] = 1;
		}
	}

	for (int p = 1; p < 16; ++p) {
		auto begin_timestamp = std::chrono::system_clock::now();

		MatMultiply(p, dim, X, Y, Z);

		auto end_timestamp = std::chrono::system_clock::now();

		std::cout << "For " << p << " number of processors, nanoseconds elapsed was: ";
		std::cout << (std::chrono::duration_cast<std::chrono::nanoseconds>(end_timestamp - begin_timestamp)).count();
	}

	for (int i = 0; i < dim; ++i) {
		delete[] X[i];
		delete[] Y[i];
		delete[] Z[i];
	}

	delete[] X;
	delete[] Y;
	delete[] Z;

	return EXIT_SUCCESS;
}