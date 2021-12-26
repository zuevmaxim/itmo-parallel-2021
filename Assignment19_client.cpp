#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <iostream>
int main(int argc, char **argv)
{
    int rank;
    int r;
    MPI_Init(&argc, &argv);
    char port_name[MPI_MAX_PORT_NAME];
    MPI_Comm intercomm;
    MPI_Status status;
    strcpy(port_name, argv[1]);
    std::cout << "Attempt to connect" << std::endl;
    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    std::cout << "Server connection" << std::endl;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    r = 42;
    MPI_Send(&r, 1, MPI_INT, 0, 0, intercomm);
    std::cout << "Client sent value: " << r << std::endl;
    MPI_Recv(&r, 1, MPI_INT, 0, 0, intercomm, &status);
    std::cout << "Client got value: " << r << std::endl;
    MPI_Finalize();
    return 0;
}