#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

std::vector<std::vector<int>> generateRandomMatrix(int size) {
    std::vector<std::vector<int>> result(size);
    for (int i = 0; i < size; ++i) {
        result[i].resize(size);
        for (int j = 0; j < size; ++j) {
            result[i][j] = std::rand();
        }
    }
    return result;
}

std::vector<std::vector<int>> multiply(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b, int threads) {
    int size = a.size();
    std::vector<std::vector<int>> c(size);
    for (int i = 0; i < size; ++i) {
        c[i].resize(size, 0);
    }
    int i, j, k;
#pragma omp parallel for num_threads(threads) private(i,j,k)
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            for (k = 0; k < size; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

void test() {
    std::cout << "===Test===\n";
}

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));

    if (argc != 2) {
        std::cerr << "One argument expected, but " << argc << " found" << std::endl;
        return 1;
    }

    int size = std::atoi(argv[1]);

    for (int threads = 1; threads <= 10; ++threads) {
        auto a = generateRandomMatrix(size);
        auto b = generateRandomMatrix(size);
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        int tries = 50;
        for (int i = 0; i < tries; i++) {
            multiply(a, b, threads);
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / (double) tries;
        std::cout << threads << ", " << time_ms << std::endl;
    }

    test();


    return 0;
}
