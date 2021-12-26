#include <stdio.h>
#include "mpi.h"
#include <iostream>
int main(int argc, char **argv)
{
    int r;
    MPI_Init(&argc, &argv);
    char port_name[MPI_MAX_PORT_NAME];
    MPI_Status status;
    MPI_Comm intercomm;
    MPI_Open_port(MPI_INFO_NULL, port_name);
    std::cout << "portname: " << port_name << std::endl;
    std::cout << "Waiting for the client ..." << std::endl;
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    std::cout << "Client connected" << std::endl;

    MPI_Recv(&r, 1, MPI_INT, 0, 0, intercomm, &status);
    std::cout << "Server got value: " << r << std::endl;
    r = 25;
    MPI_Send(&r, 1, MPI_INT, 0, 0, intercomm, &status);
    std::cout << "Server sent value: " << r << std::endl;
    MPI_Comm_free(&intercomm);
    MPI_Close_port(port_name);
    MPI_Finalize();
    return 0;
}