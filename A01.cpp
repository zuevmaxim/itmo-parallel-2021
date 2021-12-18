#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

std::vector<int> generateRandomVector(int size) {
    std::vector<int> result(size);
    for (int i = 0; i < size; ++i) {
        result[i] = std::rand();
    }
    return result;
}

int findMaximum(const std::vector<int>& data, int threads) {
    int size = data.size();
    int maximum = data[0];
#pragma omp parallel for reduction(max:maximum) num_threads(threads)
    for (int i = 0; i < size; ++i) {
        maximum = std::max(data[i], maximum);
    }
    return maximum;
}

void test() {
    std::cout << "===Test===\n";
    std::vector<int> data{2, 4, 6, 3, 8, 10, 9, 4, 2, 10};
    std::cout << "Maximum of vector [";
    for (int x : data) {
        std::cout << x << ' ';
    }
    std::cout << "] is " << findMaximum(data, 2) << '\n';
}

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));

    if (argc != 2) {
        std::cerr << "One argument expected, but " << argc << " found" << std::endl;
        return 1;
    }

    int size = std::atoi(argv[1]);

    for (int threads = 1; threads <= 10; ++threads) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::vector<int> data = generateRandomVector(size);
        int tries = 50;
        for (int i = 0; i < tries; i++) {
            findMaximum(data, threads);
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / (double) tries;
        std::cout << threads << ", " << time_ms << std::endl;
    }

    test();


    return 0;
}
