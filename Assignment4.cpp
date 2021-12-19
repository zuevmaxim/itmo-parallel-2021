#include <stdio.h>
#include "mpi.h"
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, n, i;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        int message = 42;
        int parcel;
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&parcel, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        std::cout << parcel << std::endl;
    }
    else
    {
        int message = 43;
        int parcel;
        MPI_Recv(&parcel, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
