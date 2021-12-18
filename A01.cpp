#include <iostream>
#include <vector>
#include <omp.h>


std::vector<int> generateRandomVector(int size) {
    std::vector<int> result(size);
    for (int i = 0; i < size; ++i) {
        result[i] = std::rand();
    }
    return result;
}

int findMaximum(const std::vector<int>& data, int threads) {
    return -1;
}

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));

    if (argc != 2) {
        std::cerr << "One argument expected, but " << argc << " found" << std::endl;
        return 1;
    }

    int size = std::atoi(argv[1]);
    std::vector<int> data = generateRandomVector(size);

    int maximum = data[0];

#pragma omp parallel for reduction(max:maximum)
    for (int i = 0; i < size; ++i) {
        maximum = std::max(data[i], maximum);
    }
    std::cout << maximum << std::endl;

    return 0;
}
