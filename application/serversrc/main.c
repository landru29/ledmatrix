#define DEBUG


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "daemon.h"
#include "process.h"
#include "display.h"

/**
 * Display usage of program on stdout
 *
 * @param argv Parameters of program
 *
 * @return void
 */
void usage(char **argv)
{
    fprintf(stdout, "Newspaper V1.0\n M. Hervo & C. Meichel\n2013, June\n");
    fprintf(stdout, "Syntaxe : %s\n", argv[0]);
    fprintf(stdout, "\t-s:\n\t\tSimulate the matrix\n");
    fprintf(stdout, "\t-v:\n\t\tVerbose the configuration and exit\n");
    fprintf(stdout, "\t-t index:\n\t\ttest a matrix\n");
    fprintf(stdout, "\t-c command:\n\t\texecute a daemon command (start, stop, restart)\n");
}


/**
 * Méthode de lancement du programme
 *
 * @param argc Number of parameters
 * @param argv Parameters of program
 *
 * @return status
 */
int main(int argc, char **argv)
{
    char optstring[] = "svc:t:";
    char* daemonCommand = strdup("?");
    char* testData;
    int option;
    LEDMATRIX* matrix = 0; // Espace mémoire pour l'écriture sur les matrices
    FONT** fonts = 0;
    int testMatrixIndex=-1;
    unsigned int matrixHeight; // nombre de ligne par matrice
    unsigned int matrixWidth;  // nombre de colonnes par matrice
    unsigned int displays;     // nombre de matrices
    int simulated=0;
    ANIMATIONPLUGIN** plugins;
    int daemonStatus = DAEMON_STOPPED;
    pid_t pid_child = -1;

    /* read config */
    loadConfiguration(&displays, &matrixHeight, &matrixWidth);

    opterr=0; /* Pas de message d'erreur automatique */

    if (argc < 2) {
        usage(argv);
        return 0;
    }

    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 's':
                simulated = 1;
                break;
            case 'v':
                fprintf(stdout, "Nb matrix: %d\n", displays);
                fprintf(stdout, "Matrix height: %d\n", matrixHeight);
                fprintf(stdout, "Matrix width: %d\n", matrixWidth);
                fprintf(stdout, "CS0: %d\nCS1: %d\nCS2: %d\nCS3: %d\n", getCs(0), getCs(1), getCs(2), getCs(3));
                abort();
                break;
            case 't':
            #ifdef __arm__
                sscanf(optarg, "%d", &testMatrixIndex);
                fprintf(stdout, "Testing matrix #%d\n", testMatrixIndex);
                testData = (char*)malloc(matrixHeight*matrixWidth/8);
                memset(testData, 0xff, matrixHeight*matrixWidth/8);
                writeScreen(testMatrixIndex, testData, matrixHeight*matrixWidth/8);
                free(testData);
            #endif
                break;
            case 'c':
                daemonCommand = strdup(optarg);
                break;
            default:
                usage(argv);
                return 0;
                break;
        }
    }
#ifndef __arm__
    simulated = 1;
#endif

    /* Check if launched by root */
    if (getuid() !=0) {
        fprintf(stdout, "You should be root to launch this program\n");
        return 0;
    }



    if (strcmp(daemonCommand, "start") == 0) {
        if (getMyPid()==0) {
            pid_child = daemonStart();
            daemonStatus = (pid_child >= 0) ? DAEMON_STARTED : DAEMON_STOPPED;
        } else {
            fprintf(stdout,"Daemon is still alive\n");
            daemonStatus = DAEMON_STARTED;
        }
    }

    if (strcmp(daemonCommand, "stop") == 0) {
        daemonStatus = (daemonStop() >= 0) ? DAEMON_STOPPED : DAEMON_STARTED;
    }

    if (strcmp(daemonCommand, "restart") == 0) {
        fprintf(stdout, "Restarting daemon\n");
        if (daemonStop() >0) {
            pid_child = daemonStart();
            daemonStatus = (pid_child >= 0) ? DAEMON_STARTED : DAEMON_STOPPED;
        } else {
            daemonStatus = DAEMON_STARTED;
        }
    }

    if (pid_child == 0) {
        /* Load plugins */
        plugins = loadAllPlugins();

        // read fonts
        fonts = loadFonts(0,0);

        /* Matrix initialisation */
        matrix = openLedMatrix(displays*matrixWidth, matrixHeight);
        if (simulated) {
            matrixSetDebugMode(matrix, 1);
            matrixDebugInit();
        }
        // main loop
        mainLoop(matrix, plugins, fonts);

        // Cleaning everything
        unloadFonts(fonts);

        closeLedMatrix(matrix);
        closePlugins(plugins);
    }

    return 0;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/
