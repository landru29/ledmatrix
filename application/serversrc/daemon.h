#ifndef DAEMON_H_INCLUDED
#define DAEMON_H_INCLUDED


#include <unistd.h>
#include <sys/types.h>

#define DAEMON_STOPPED 0
#define DAEMON_STARTED 1


/**
 * Get the PID from the current running daemon
 *
 * @return the PID if exists otherwhise 0
 */
pid_t getMyPid();

/**
 * Register a PID
 */
void registerMyPid();

/**
 * Destroy a registered PID
 */
pid_t destroyMyPid();

/**
 * Start the daemon
 *
 * @return pid
 */
pid_t daemonStart();

/**
 * Start the daemon
 *
 * @return status
 */
int daemonStop();


#endif // DAEMON_H_INCLUDED
