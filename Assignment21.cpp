#include <stdio.h>
#include "mpi.h"

#define BUFFER_SIZE 10

void createFile(char* filename) {
    MPI_Status status;
    MPI_File fh;
    MPI_File_open(MPI_COMM_SELF, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    char* buf = "Hello world!\nHello, Hello, Hello world!\nHello, Hello, Hello world!\nHello, Hello, Hello world!\nHello, Hello";
    MPI_File_write(fh, buf, strlen(buf), MPI_CHAR, &status);
    MPI_File_close(&fh);
}

int main(int argc, char **argv)
{
    int rank;
    MPI_Init(&argc, &argv);
    MPI_File fh;
    char filename[10] = "file.txt";
    char buf[BUFFER_SIZE];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        createFile(filename);
    }
    MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, rank * BUFFER_SIZE, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    MPI_File_read_all(fh, buf, BUFFER_SIZE, MPI_CHAR, MPI_STATUS_IGNORE);
    printf("process %d, buf=%.*s\n", rank, strlen(buf), buf);
    MPI_File_close(&fh);
    MPI_Finalize();
}
