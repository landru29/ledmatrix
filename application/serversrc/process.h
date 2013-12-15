#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include "font.h"
#include "ledmatrix.h"
#include "animate.h"
#include "plugins.h"
#include "ini.h"
#include "display.h"
#include "constant.h"

#define COMMAND_BAD 0
#define COMMAND_MSG 1
#define COMMAND_QUIT 2
#define COMMAND_FONT_SELECT 3

/**
 * Charge la configuration depuis conf.ini
 *
 * @param displays nombre de matrices
 * @param matrixHeight hauteur de matrice en pixels
 * @param matrixWidth largeur de matrice en pixels
 *
 * @return void
 */
void loadConfiguration(unsigned int* displays, unsigned int* matrixHeight, unsigned int* matrixWidth);

/**
 * Check if matrix must be tested and test it
 *
 * @param testMatrixIndex matrix index to test
 * @param matrixHeight matrix height in dots
 * @param matrixWidth matrix width in dots
 *
 * @return 1 if tested otherwise 0
 */
int checkMatrixToTest(int testMatrixIndex, unsigned int matrixHeight, unsigned int matrixWidth);

/**
 * Load plugins
 *
 * @return plugins collections
 */
ANIMATIONPLUGIN** loadAllPlugins();

/**
 * Create a node file for communication client-server
 */
void createNodes();

/**
 * destroy the communication nodes
 */
void destroyNodes();

/**
 * Load fonts
 *
 * @return font collections
 */
FONT** loadFonts();

/**
 * close all fonts and release memory
 *
 * @param fonts list of all loaded fonts
 */
void unloadFonts(FONT** fonts);

/**
 * Select a font by index
 *
 * @param matrix       ledmatrix to assign font
 * @param fonts        fonts collection
 * @param fontSelector index of the font
 *
 * @return number of fonts
 */
int selectFont(LEDMATRIX* matrix, FONT** fonts, int fontSelector);

/**
 * Open the communication pipe
 */
FILE** openCommunicationPipe();

/**
 * Close the communication pipe
 */
void closeCommunicationPipe(FILE** pipe);

/**
 * Check if a file exists
 *
 * @param filename file to check
 *
 * @return true if exists
 */
int file_exists (char * fileName);

/**
 * Send a message without any animation
 *
 * @param matrix  the matrix to present the message
 * @param message the message to display
 */
void quickMessage(LEDMATRIX* matrix, char* message);

/**
 * Extract a command from a data
 *
 * @param data     data to parse
 * @param command  parsed command
 * @param argument parsed argument
 * @param len      length of the argument and command
 *
 * @return 0 on success
 */
int commandParse(char* data, char* command, char* argument, size_t len);

/**
 * Daemon main loop
 *
 * @param matrix  the matrix !
 * @param plugins plugins collection
 * @param fonts   fonts collection
 *
 * @return status
 */
int mainLoop(LEDMATRIX* matrix, ANIMATIONPLUGIN** plugins, FONT** fonts);

/**
 * Read a command from the pipe
 *
 * @param pipe Pipe to read
 * @param data Data passed from the command
 *
 * @return command Identifier
 */
int readCommand(FILE* pipe, char* data, size_t len);


#endif // DAEMON_H_INCLUDED
