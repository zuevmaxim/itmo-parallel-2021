#include <iostream>
#include <mpi.h>
#include <vector>
using namespace std;

std::vector<int> generateVector(int size) {
    std::vector<int> result(size, 1);
    return result;
}

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int n = 1000000;
        auto a = generateVector(n);
        auto b = generateVector(n);
        int block = n / (size - 1);
        for (int i = 1; i < size; ++i) {
            int length = i == size - 1 ? n - (i - 1) * block : block;
            MPI_Send(&length, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a[(i - 1) * block], length, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b[(i - 1) * block], length, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        int dot = 0;
        for (int i = 1; i < size; ++i) {
            int dot_local;
            MPI_Recv(&dot_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            dot += dot_local;
        }
        cout << dot << endl;
    } else {
        int length;
        MPI_Recv(&length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::vector<int> a(length, 0);
        std::vector<int> b(length, 0);
        MPI_Recv(&a[0], length, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&b[0], length, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int dot = 0;
        for (int i =0; i < length; ++i) {
            dot += a[i] * b[i];
        }
        cout << rank << ' ' << dot << '\n';
        MPI_Send(&dot, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
