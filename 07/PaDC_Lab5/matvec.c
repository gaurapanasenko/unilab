#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

#define MPI_WTIME_IS_GLOBAL 1

const int BUFF_SIZE = 2048;

int gen_array(int size, int k, int (*matrix)[size]) {
    int min_size = size < 16 ? size : 16, i, j, a;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++) {
            a = i * size + j;
            matrix[a%k + a/(size*k)*k][a%(size*k)/k] = rand() % 16;
        }

    printf("Matrix <=16x16:\n");
    for (i = 0; i < min_size; i++) {
        for (j = 0; j < min_size; j++) {
            printf("%5i ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int gen_and_broadcast_array(int comm_rank, int comm_size, int k, int (*matrix_part)[comm_size * k], int * vector) {
    // Calculate sizes
    int size = comm_size * k;
    int part_size = k * size;
    int min_size = size < 16 ? size : 16;

    if (comm_rank == 0) {
        // Initalize matrix and vector and allocate memory for them
        int (*matrix)[size], *new_vector;
        matrix = (int (*)[size])malloc(size * size * sizeof(int));
        new_vector = (int *)malloc(size * sizeof(int));

        // Generate data
        gen_array(size, k, matrix);
        printf("Vector:\n");
        for (int i = 0; i < size; i++) {
            new_vector[i] = rand() % 16;
            if (i < 16)
                printf("%5i ", new_vector[i]);
        }
        printf("\n\n");

        // Broadcast data
        memcpy(matrix_part, matrix, part_size * sizeof(int));
        memcpy(vector, new_vector, size * sizeof(int));
        for (int i = 1; i < comm_size; i++) {
            MPI_Send(matrix + k * i, part_size, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
            MPI_Send(new_vector, size, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
        }

        // Free memory
        free(matrix);
        free(new_vector);
    } else {
        MPI_Recv(matrix_part, part_size, MPI_INT, 0, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(vector, size, MPI_INT, 0, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

int worker(int argc, char* argv[], int comm_rank, int comm_size) {
    // Calculate sizes
    int k = atoi(argv[1]);
    int size = comm_size * k;
    int part_size = comm_size * size;

    // Initialize needed variables
    int (*matrix_part)[size], *vector, *res_vector;
    double start, end;

    // Allocate memory for matrix and vectors
    matrix_part = (int (*)[size])malloc(k * size * sizeof(int));
    vector = (int *)malloc(size * sizeof(int));
    res_vector = (int *)calloc(size, sizeof(int));

    // Generate and broadcast matrix
    gen_and_broadcast_array(comm_rank, comm_size, k, matrix_part, vector);

    // Calculate
    start = MPI_Wtime();
    for (int i = 0; i < k; i++)
        for (int j = 0; j < size; j++)
            res_vector[i + comm_rank * k] += matrix_part[i][j] * vector[j];
    end = MPI_Wtime();

    // Sync rest of result vector with other processes
    for (int i = 0; i < comm_size; i++) {
        if (i > comm_rank) {
            MPI_Send(res_vector + comm_rank * k, k, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
            MPI_Recv(res_vector + i * k, k, MPI_INT, i, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (i < comm_rank) {
            MPI_Recv(res_vector + i * k, k, MPI_INT, i, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(res_vector + comm_rank * k, k, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
        }
    }

    // Print result data
    printf("Res vector in rank %4i:", comm_rank);
    size = size < 16 ? size : 16;
    for (int i = 0; i < size; i++)
        printf("%5i ", res_vector[i]);
    printf(". Run in %f s.\n", end - start);
    fflush(stdout);

    // Free allocated memory
    free(matrix_part);
    free(vector);
    free(res_vector);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s must have numeric argument, "
                        "that will represent size matrix (k)\n",
                argv[0]);
        return 1;
    }
    int status;
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    status = worker(argc, argv, rank, size);

    MPI_Finalize();
    return status;
}
