#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int get_number_of_bytes(char *filename);
int printnbytes(char *filename);
pid_t printnbytes_fork(char *filename);


/** @brief Given a file path, returns the number of bytes in it.
 * @param filename A valid file path
 * @return The number of bytes, or -1 if there was an error reading.
 */
int get_number_of_bytes(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)  return -1;

    int nchars = 0;
    for(fgetc(fp); !feof(fp); fgetc(fp))  nchars++;

    fclose(fp);

    return nchars;
}

int printnbytes(char *filename)
{
    int nbytes = get_number_of_bytes(filename);
    if (nbytes < 0) {
        fprintf(stderr, "Error reading from file '%s'. Check existence and access permissions.\n", filename);
        return 1;
    }
    printf("Number of bytes in file '%s': %d\n", filename, nbytes);
    return 0;
}

pid_t printnbytes_fork(char *filename)
{
    pid_t pid = fork();

    if (pid == -1) { // Error only
        fprintf(stderr, "Error forking.\n");
    }
    if (pid == 0) { // Child only
        printnbytes(filename);
        exit(EXIT_SUCCESS);
    }
    
    // Parent only
    return pid;
}


int main(int argc, char **argv)
{
    // No minimum args

    // Fork for each argument given
    for (int i=0; i<argc-1; i++)  printnbytes_fork(argv[i+1]);
    
    // Wait for all processes
    for (int i=0; i<argc-1; i++) {
        int status;
        pid_t pid = wait(&status);
        if (status != EXIT_SUCCESS)  fprintf(stderr, "Process %d failed with status %d\n", pid, status);
    }

    exit(EXIT_SUCCESS);
}

