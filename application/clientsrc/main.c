#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../serversrc/constant.h"








int main(int argc, char** argv)
{
    FILE* fifoFile;
    int fifoFd;

    if ((fifoFd = open(NODE_NAME, O_WRONLY)) < 0) {
        fprintf(stderr, "Could not open the FIFO\n");
        exit(EXIT_FAILURE);
    }
    fifoFile = fdopen(fifoFd, "w");

    if (argc !=2) {
        fprintf(stderr, "Please specify the message\n");
    }

    fprintf(stdout, "Sending message : %s\n", argv[1]);
    fprintf(fifoFile, "%s\n", argv[1]);

    fclose(fifoFile);
    return EXIT_SUCCESS;
}
