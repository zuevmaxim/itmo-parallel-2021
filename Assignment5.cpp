#include <iostream>
#include "mpi.h"
#define NTIMES 100
using namespace std;
int main(int argc, char **argv)
{
    double time_start, time_finish;
    int rank, i;
    int len;
    char *name = new char[100];
    MPI_Init(&argc, &argv);
    time_start = MPI_Wtime();
    for (i = 0; i < NTIMES; i++)
    {
        int rank, n, i;
        MPI_Status status;
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
    }
    time_finish = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &len);
    cout << "processor " << name << ", process " << rank << "time = " << (time_finish - time_start) / NTIMES << endl;
    MPI_Finalize();
}