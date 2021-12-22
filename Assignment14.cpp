#include <stdio.h>
#include "mpi.h"

#define n 1000

void max(void* in, void* out, int* l, MPI_Datatype* type) {
    int* a = (int*) in;
    int* b = (int*) out;
    for (int i = 0; i < *l; i++) {
        if (a[i] > b[i]) {
            b[i] = a[i];
        }
    }
}

int main(int argc, char** argv) {
    int rank, size, i;
    int a[n];
    int b1[n] = {0};
    int b2[n] = {0};
    MPI_Init(&argc, &argv);
    MPI_Op op;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (i = 0; i < n; i++) {
        a[i] = i + rank + 1;
    }
    MPI_Op_create(&max, 1, &op);
    MPI_Reduce(a, b1, n, MPI_INT, op, 0, MPI_COMM_WORLD);
    MPI_Reduce(a, b2, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Op_free(&op);

    if (rank == 0) {
        std::cout << b1[0] << ' ' << b2[0];
    }

    MPI_Finalize();
}
