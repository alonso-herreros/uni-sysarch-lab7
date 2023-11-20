#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CHILDREN 3
#define BUFFER_SIZE 1024


int get_number_of_lines(char *filename);
int printnlines(char *filename);
// pid_t printnlines_fork(char *filename);


/** @brief Given a file path, returns the number of lines in it.
 * @param filename A valid file path
 * @return The number of lines, or -1 if there was an error reading.
 */
int get_number_of_lines(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)  return -1;

    int nlines = 0;
    size_t nchars = BUFFER_SIZE;
    char *line = malloc(nchars*sizeof(char));
    while (!feof(fp)) {
        getline(&line, &nchars, fp);
        if (ferror(fp)) {
            nlines = -1; break;
        }
        nlines++;
    }

    free(line);
    fclose(fp);

    return nlines;
}

int printnlines(char *filename)
{
    int nlines = get_number_of_lines(filename);
    if (nlines < 0) {
        fprintf(stderr, "Error reading from file '%s'. Check existence and access permissions.\n", filename);
        return 1;
    }
    printf("Number of lines in file '%s': %d\n", filename, nlines);
    return 0;
}

pid_t printnlines_fork(char *filename)
{
    pid_t pid = fork();

    if (pid == -1) { // Error only
        fprintf(stderr, "Error forking.\n");
    }
    if (pid == 0) { // Child only
        printnlines(filename);
        exit(EXIT_SUCCESS);
    }
    
    // Parent only
    return pid;
}


int main(int argc, char **argv)
{
    // Fork for each argument given
    int children = 0;
    int argi = 1;
    while (1) {
        // Forking: if there are more args and can create children
        if (argi<argc && children < MAX_CHILDREN) {
            // printf("Forked %d (%d running)\n", printnlines_fork(argv[argi++]), ++children); // LOGGING
            pid_t pid = printnlines_fork(argv[argi++]);
            if (pid > 0) children++;
        } // We skip waiting here to spawn all children ASAP
        // Waiting: if no child was created this loop, and there are children
        else if (children > 0) {
            int status;
            pid_t pid = wait(&status);
            if (status != EXIT_SUCCESS)  fprintf(stderr, "Process %d failed with status %d\n", pid, status);
            // else printf("Process %d succeeded (%d running)\n", pid, --children); // LOGGING
            children--;
        }
        // When there are no more children, exit
        else  break; 
    }

    exit(EXIT_SUCCESS);
}

