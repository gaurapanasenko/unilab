#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int worker(int argc, char* argv[], int rank, int size, char *hostname, int host_len) {
    MPI_File stream;
    int ierr, data_size;
    char *data;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s infilepath\n", argv[0]);
        return 1;
    }
    ierr = MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &stream);
    if (ierr) {
        if (rank == 0)
            fprintf(stderr, "%s: Couldn't open file %s\n", argv[0], argv[1]);
        return 2;
    }

    /* read in relevant chunk of file into "chunk",
     * which starts at location in the file globalstart
     * and has size mysize
     */
    {
        MPI_Offset globalstart, globalend, filesize;

        /* figure out who reads what */
        MPI_File_get_size(stream, &filesize);
        filesize--;  /* get rid of text file eof */
        data_size = filesize / size;
        globalstart = rank * data_size;
        globalend   = globalstart + data_size - 1;
        if (rank == size-1) globalend = filesize-1;

        data_size =  globalend - globalstart + 1;

        /* allocate memory */
        MPI_Alloc_mem(data_size + 1, MPI_INFO_NULL, &data);

        /* everyone reads in their part */
        MPI_File_read_at_all(stream, globalstart, data, data_size, MPI_CHAR, MPI_STATUS_IGNORE);
        data[data_size] = '\0';
    }

    printf("Hostname: %s, number of running processes: %i, rank: %i, data:\n%s\n\n", hostname, size, rank, data);
    fflush(stdout);

    MPI_Free_mem(data);
    MPI_File_close(&stream);
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
