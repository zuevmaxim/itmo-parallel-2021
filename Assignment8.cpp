#include <iostream>
#include "mpi.h"
#include <vector>
#define NTIMES 1000
#define LTIMES 10000000
using namespace std;

std::vector<int> generateRandomVector(int size) {
    std::vector<int> result(size);
    for (int i = 0; i < size; ++i) {
        result[i] = std::rand();
    }
    return result;
}

int main(int argc, char **argv)
{
    double time_start, time_finish;
    int rank;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (int size = 1; size <= (int)1e6; size *= 10) {
        std::vector<int> data = generateRandomVector(size);
        time_start = MPI_Wtime();
        for (int i = 0; i < NTIMES; ++i)
        {
            if (rank == 0)
            {
                MPI_Send(&data[0], size, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&data[0], size, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            }
            else
            {
                MPI_Recv(&data[0], size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(&data[0], size, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
        time_finish = MPI_Wtime();
        double time_s = time_finish - time_start;
        double R = 2 * NTIMES * size * sizeof(int) / time_s / (1 << 20);
        if (rank == 0) {
            cout << "size=" <<  size << " time=" << time_s << " bandwidth=" << R << endl;
        }
    }

    time_start = MPI_Wtime();
    int message = 0;
    for (int i = 0; i < LTIMES; ++i)
    {
        if (rank == 0)
        {
            MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        }
        else
        {
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    time_finish = MPI_Wtime();
    double time_s = time_finish - time_start;
    double L = time_s / (2 * LTIMES);
    if (rank == 0) {
        cout << "time=" << time_s << " latency=" << L << endl;
    }

    MPI_Finalize();
}
