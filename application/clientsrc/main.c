#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../serversrc/constant.h"


int main(int argc, char** argv)
{
    FILE* outFile;
    FILE* inFile;
    int outFd;
    int inFd;
    char answer[200] = "";

    if ((outFd = open(NODE_NAME, O_WRONLY)) < 0) {
        fprintf(stderr, "Could not open the OUT FIFO %s\n", NODE_NAME);
        exit(EXIT_FAILURE);
    }
    outFile = fdopen(outFd, "w");

    if ((inFd = open(RETURN_NODE_NAME, O_RDONLY)) < 0) {
        fprintf(stderr, "Could not open the IN FIFO\n");
        exit(EXIT_FAILURE);
    }
    inFile = fdopen(inFd, "r");

    if (argc !=2) {
        fprintf(stderr, "Please specify the command (command:argument)\n");
        fclose(outFile);
        fclose(inFile);
        return 0;
    }

    fprintf(stdout, "Sending message : %s\n", argv[1]);
    fprintf(outFile, "%s\n", argv[1]);

    /*if (fgets(answer, 200, inFile)) {
        fprintf(stdout, "%s", answer);
    }*/

    fclose(outFile);
    //fclose(inFile);
    return EXIT_SUCCESS;
}
