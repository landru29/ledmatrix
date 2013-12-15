#include <stdio.h>
#include "daemon.h"
#include "constant.h"
#include <string.h>
#include <signal.h>
#include "process.h"

/**
 * Get the PID from the current running daemon
 *
 * @return the PID if exists otherwhise 0
 */
pid_t getMyPid()
{
    FILE* f;
    char strPid[200];
    int pid;
    if ((f = fopen(PID_FILENAME, "r"))) {
        if (fgets(strPid, 200, f)) {
            if (strlen(strPid) ==0) {
                return 0;
            }
        }
        fclose(f);
        sscanf(strPid, "%d", &pid);
        return (pid_t)pid;
    } else {
        return 0;
    }
}

/**
 * Register a PID
 */
void registerMyPid()
{
    FILE* f;
    if ((f = fopen(PID_FILENAME, "w"))) {
        fprintf(f, "%d", (int)getpid());
        fflush(f);
        fclose(f);
    }
}

/**
 * Destroy a registered PID
 */
pid_t destroyMyPid()
{
    pid_t pid;
    pid = getMyPid();
    if (pid) {
        unlink(PID_FILENAME);
    }
    return pid;
}

/**
 * Start the daemon
 *
 * @return pid
 */
pid_t daemonStart()
{
    pid_t pid;
    fprintf(stdout, "Starting deamon ledMatrix... ");
    fflush(stdout);
    pid = fork();
    if (pid == 0) {
        // we are in the child process
        setsid();
        registerMyPid();
        fprintf(stdout, "OK\n");
    }
    if (pid<0) {
        fprintf(stdout, "Fail\n");
    }
    return pid;
}

/**
 * Start the daemon
 *
 * @return status
 */
int daemonStop()
{
    pid_t pid;
    fprintf(stdout, "Stopping deamon ledMatrix... ");
    pid = destroyMyPid();
    if (pid>0) {
        kill(pid, SIGKILL);
        fprintf(stdout, "OK\n");
        return 1;
    }
    fprintf(stdout, "Fail\n");
    return -1;
}

