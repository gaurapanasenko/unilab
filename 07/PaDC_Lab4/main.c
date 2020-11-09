#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int worker(int argc, char* argv[], int rank, int size, char *hostname, int host_len) {
    int ierr, data_size, sum = 0, data[32], i;

    switch (rank) {
    case 1:
        MPI_Recv(data, 32, MPI_INT, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i = 0; i < 32; i++) {
            sum += data[i];
        }
        for (i = 0; i < size; i++) {
            if (i != rank)
                MPI_Send(&sum, 1, MPI_INT, i, 11, MPI_COMM_WORLD);
        }
    break;
    case 0:
        for (i = 0; i < 32; i++) {
            data[i] = i;
        }
        MPI_Send(data, 32, MPI_INT, 1, 10, MPI_COMM_WORLD);
    default:
        MPI_Recv(&sum, 32, MPI_INT, 1, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Sum: %i, Rank: %i\n", sum, rank);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int status;
    MPI_Init(&argc, &argv);

    int rank, size, host_len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(hostname, &host_len);
    status = worker(argc, argv, rank, size, hostname, host_len);

    MPI_Finalize();
    return status;
}
