#include <stdio.h>
#include "mpi.h"

#define BUFSIZE 100


void createFile(char* filename) {
    MPI_Status status;
    MPI_File fh;
    MPI_File_open(MPI_COMM_SELF, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    char buf[42] = "Hello world!\nHello, Hello";
    MPI_File_write(fh, buf, strlen(buf), MPI_CHAR, &status);
    MPI_File_close(&fh);
}

void deleteFile(char* filename) {
    MPI_File_delete(filename, MPI_INFO_NULL);
}


int main(int argc, char** argv) {
    int bufsize, num, sum;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_File fh;
    char buf[BUFSIZE];
    char filename[10] = "file.txt";
    createFile(filename);
    MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    sum = 0;
    do {
        MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
        MPI_Get_count(&status, MPI_CHAR, &num);
        printf("buf=%.*s\n", num,  buf);
        sum += num;
    } while (num >= BUFSIZE);
    MPI_File_close(&fh);
    if (argc > 1 && strcmp(argv[1], "delete") == 0) {
        deleteFile(filename);
    }
    printf("read symbols=%d\n", sum);
    MPI_Finalize();
}
