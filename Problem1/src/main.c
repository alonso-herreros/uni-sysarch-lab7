#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int get_number_of_bytes(char *filename);

/** @brief Given a file path, returns the number of bytes in it.
 * @param filename A valid file path
 * @return The number of bytes, or -1 if there was an error reading.
 */
int get_number_of_bytes(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)  return -1;

    int nchars = 0;
    for(fgetc(fp); !feof(fp); fgetc(fp)) {
        nchars++;
    }

    fclose(fp);

    return nchars;
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments: 1 expected, %d given\n", argc-1);
        exit(EXIT_FAILURE);
    }

    char *filepath = argv[1];
    int nbytes = get_number_of_bytes(filepath);
    if (nbytes < 0) {
        fprintf(stderr, "Error reading from file '%s'. Check existence and access permissions.\n", filepath);
        exit(EXIT_FAILURE);
    }

    printf("Number of bytes in file '%s': %d\n", filepath, nbytes);

    exit(EXIT_SUCCESS);
}

