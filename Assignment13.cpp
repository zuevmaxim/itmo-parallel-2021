#include <cstdlib>
#include "mpi.h"
#include <vector>


std::vector <std::vector<int>> generateRandomMatrix(int size) {
    std::vector <std::vector<int>> result(size);
    for (int i = 0; i < size; ++i) {
        result[i].resize(size);
        for (int j = 0; j < size; ++j) {
            result[i][j] = std::rand();
        }
    }
    return result;
}

std::vector <std::vector<int>>
multiply(const std::vector <std::vector<int>>& a, const std::vector <std::vector<int>>& b) {
    int size = a.size();
    std::vector <std::vector<int>> c(size);
    for (int i = 0; i < size; ++i) {
        c[i].resize(size, 0);
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

int main(int argc, char** argv) {
    int rank, size;
    double time_start, time_finish;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//matrix initialization by each process
    auto a = generateRandomMatrix(500);
    auto b = generateRandomMatrix(500);
    MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization
// start timing for each process
    time_start = MPI_Wtime();
// matrix multiplication
    int repeats = 10;
    for (int i = 0; i < repeats; ++i) {
        multiply(a, b);
    }
// end of timing
    time_finish = MPI_Wtime();
// output the execution time of matrix multiplication at each process
    double time_s = (time_finish - time_start) / repeats;
    std::cout << rank << ' ' << time_s << std::endl;
    MPI_Finalize();
}