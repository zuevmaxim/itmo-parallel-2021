#include <iostream>
#include "mpi.h"
#include <vector>

#define NTIMES 1000
using namespace std;

std::vector<int> generateVector(int size) {
    std::vector<int> result(size, 1);
    return result;
}

int main(int argc, char** argv) {
    double time_start, time_finish;
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> array;
    if (rank == 0) {
        int n = 1000000;
        int block = n / size;
        array = generateVector(block);
        std::vector<int> data = generateVector(n - block);
        for (int i = 1; i < size; ++i) {
            int length = i == size - 1 ? n - block * (size - 1) : block;
            MPI_Send(&length, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&data[block * (i - 1)], length, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        int length;
        MPI_Recv(&length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        array.resize(length);
        MPI_Recv(&array[0], length, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int sum = 0;
    for (int x : array) {
        sum += x;
    }

    int total;
    time_start = MPI_Wtime();
    for (int i = 0; i < NTIMES; ++i) {
        total = 0;
        MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    time_finish = MPI_Wtime();
    double time_s = (time_finish - time_start) / NTIMES;
    if (rank == 0) {
        std::cout << "Reduce " << total << ' ' << time_s << std::endl;
    }


    time_start = MPI_Wtime();
    for (int i = 0; i < NTIMES; ++i) {
        total = sum;
        int level = 1;
        while (true) {
            int blockCount = 1 << (level - 1);
            if (((rank >> (level - 1)) & 1) == 0) {
                int neighbour = rank + blockCount;
                if (neighbour < size) {
                    int local;
                    MPI_Recv(&local, 1, MPI_INT, neighbour, 0, MPI_COMM_WORLD, &status);
                    total += local;
                } else if (rank == 0) break;
            } else {
                int neighbour = rank - blockCount;
                MPI_Send(&total, 1, MPI_INT, neighbour, 0, MPI_COMM_WORLD);
                break;
            }
            level++;
        }
    }
    time_finish = MPI_Wtime();
    time_s = (time_finish - time_start) / NTIMES;
    if (rank == 0) {
        std::cout << "Cascade " << total << ' ' << time_s << std::endl;
    }

    MPI_Finalize();
}
