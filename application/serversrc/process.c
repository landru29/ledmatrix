#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <regex.h>
#include "font.h"
#include "ledmatrix.h"
#include "animate.h"
#include "plugins.h"
#include "ini.h"
#include "display.h"
#include "constant.h"
#include "process.h"
#include "tools.h"

/**
 * Construit le chemin vers le dossier des plugins
 *
 * @return chemin absolu vers les plugins
 */
char* pluginsFolder()
{
	return strdup(LIBDIR);
}

/**
 * Construit le chemin du fichi ini
 *
 * @return chemin absolu vers le fichier ini
 */
char* iniFile()
{
    char confPath[200];
    sprintf(confPath, "%s/conf.ini", CONFDIR);
    return strdup(confPath);
}

/**
 * Construit le chemin d'une font
 *
 * @return chemin absolu vers le fichier de font
 */
char* fontFile(char* basename)
{
    char confPath[200];
    sprintf(confPath, "%s/%s", FONTDIR, basename);
    return strdup(confPath);
}

/**
 * Charge la configuration depuis conf.ini
 *
 * @param displays nombre de matrices
 * @param matrixHeight hauteur de matrice en pixels
 * @param matrixWidth largeur de matrice en pixels
 *
 * @return void
 */
void loadConfiguration(unsigned int* displays, unsigned int* matrixHeight, unsigned int* matrixWidth)
{
    INI_LINE* configuration;
    int csValueTmp;
    /* read config */
    configuration = iniParse(iniFile());
    /* retrieve data from configuration */
    if (iniHasKey(configuration, "displays")) {
        sscanf(iniGet(configuration, "displays"), "%d", displays);
    } else {
        *displays = 3;
    }
    if (iniHasKey(configuration, "matrixHeight")) {
        sscanf(iniGet(configuration, "matrixHeight"), "%d", matrixHeight);
    } else {
        *matrixHeight = 8;
    }
    if (iniHasKey(configuration, "matrixWidth")) {
        sscanf(iniGet(configuration, "matrixWidth"), "%d", matrixWidth);
    } else {
        *matrixWidth = 32;
    }
    if (iniHasKey(configuration, "CS0")) {
        sscanf(iniGet(configuration, "CS0"), "%d", &csValueTmp);
        setCs(0, (unsigned char)csValueTmp);
    }
    if (iniHasKey(configuration, "CS1")) {
        sscanf(iniGet(configuration, "CS1"), "%d", &csValueTmp);
        setCs(1, (unsigned char)csValueTmp);
    }
    if (iniHasKey(configuration, "CS2")) {
        sscanf(iniGet(configuration, "CS2"), "%d", &csValueTmp);
        setCs(2, (unsigned char)csValueTmp);
    }
    if (iniHasKey(configuration, "CS3")) {
        sscanf(iniGet(configuration, "CS3"), "%d", &csValueTmp);
        setCs(3, (unsigned char)csValueTmp);
    }

    /* Release configuration */
    iniDestroy(configuration);
}

/**
 * Check if matrix must be tested and test it
 *
 * @param testMatrixIndex matrix index to test
 * @param matrixHeight matrix height in dots
 * @param matrixWidth matrix width in dots
 *
 * @return 1 if tested otherwise 0
 */
int checkMatrixToTest(int testMatrixIndex, unsigned int matrixHeight, unsigned int matrixWidth)
{
    char* dataTest;
    if (testMatrixIndex>=0) {
        fprintf(stdout, "Testing matrix %d\n", testMatrixIndex);
        dataTest = (char*) malloc(matrixWidth*matrixHeight/8);
        memset(dataTest, 0xff, matrixWidth*matrixHeight/8);
        writeScreen(testMatrixIndex, dataTest, matrixWidth*matrixHeight/8, matrixWidth, matrixHeight);
        free (dataTest);
        return 1;
    }
    return 0;
}

/**
 * Load plugins
 *
 * @return plugins collections
 */
ANIMATIONPLUGIN** loadAllPlugins()
{
    unsigned int i;
    ANIMATIONPLUGIN** plugins;
    /* Load plugins */
    plugins = loadPlugins(pluginsFolder());
	if (!plugins) {
		printf("No plugins ! Exiting.\n");
		return 0;
	}
    for(i=0; plugins[i]; i++)
		printf("%s was loaded\n", plugins[i]->name);
    return plugins;
}




/**
 * Create two nodes file for communication client-server
 */
void createNodes()
{
    mode_t oldMask = umask(0040);
    if (mkfifo(NODE_NAME, 0642) !=0) {
        fprintf(stderr, "Could not create the FIFO IN node\n");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(RETURN_NODE_NAME, 0644) !=0) {
        fprintf(stderr, "Could not create the FIFO OUT node\n");
        exit(EXIT_FAILURE);
    }
    umask(oldMask);
}

/**
 * destroy the communication nodes
 */
void destroyNodes()
{
    unlink(NODE_NAME);
    unlink(RETURN_NODE_NAME);
}

/**
 * Load fonts
 *
 * @return font collections
 */
FONT** loadFonts(char* folder, FONT** fonts)
{
    unsigned int len=0;
    DIR* dir;
    struct dirent* entrees;
    struct stat statbuf;
    char fontPath[500];
    if (!fonts) {
        fonts = (FONT**) malloc(sizeof(FONT*));
        fonts[0]=0;
        fprintf(stdout, "Loading fonts\n");
    }
    while(fonts[len]) len ++;
    if (!folder) {
        folder = strdup(FONTDIR);
    }
    printf("  * reading %s\n", folder);

    if ((dir = opendir(folder)) == 0) {
		fprintf(stderr, "Could not find font path %s\n", folder);
		return fonts;
	}

	while (entrees = readdir(dir)) {
        sprintf(fontPath, "%s/%s", folder, entrees->d_name);
        stat(fontPath, &statbuf);
        if (entrees->d_name[strlen(entrees->d_name)-1] =='.') continue;
        if (S_ISDIR(statbuf.st_mode)) {
            loadFonts(fontPath, fonts);
            continue;
        }
        if (strcmp(getExtension(entrees->d_name), "font") == 0) {
            len++;
            fonts = (FONT**) realloc(fonts, (len+1)*sizeof(FONT*));
            printf("      => %d Loading %s\n", len, fontPath);
            fonts[len-1] = loadFont(fontPath);
            fonts[len] = 0;
            continue;
        }
	}
    printf("Done.\n");
    return fonts;
}

/**
 * Select a font by index
 *
 * @param matrix       ledmatrix to assign font
 * @param fonts        fonts collection
 * @param fontSelector index of the font
 *
 * @return number of fonts
 */
int selectFont(LEDMATRIX* matrix, FONT** fonts, int fontSelector)
{
    unsigned int i;
    /* Select font */
    for (i=0; fonts[i]; i++);
    fontSelector = (fontSelector<i?fontSelector:0);
    if (fontSelector>=0) {
        matrixSetFont(matrix, fonts[fontSelector]);
    } else {
        matrixSetFont(matrix, fonts[i-1]);
    }
    return i;
}

/**
 * close all fonts and release memory
 *
 * @param fonts list of all loaded fonts
 */
void unloadFonts(FONT** fonts)
{
    unsigned int i;
    for (i=0; fonts[i]; i++)
    destroyFont(fonts[i]);
    free(fonts);
}

/**
 * Open the communication pipe
 */
FILE** openCommunicationPipe()
{
    int fifoFd;
    FILE* fifoFile;
    int returnFd;
    FILE* returnFile;
    FILE** files = (FILE**)malloc(sizeof(FILE*)*2);
    printf("Opening communication nodes\n");
    if (!file_exists(NODE_NAME)) {
        // Create the node if not exists
        createNodes();
    }
    // Open the FIFO
    fifoFd = open(NODE_NAME, O_RDWR);
    returnFd = open(RETURN_NODE_NAME, O_RDWR);
    files[0] = fdopen(fifoFd, "r");
    //files[1] = fdopen(returnFd, "w");
    return files;
}

/**
 * Close the communication pipe
 */
void closeCommunicationPipe(FILE** pipe)
{
    // close the FIFO
    printf("Closing communication nodes\n");
    fclose(pipe[0]);
    //fclose(pipe[1]);
    free(pipe);
    destroyNodes();
}

/**
 * Check if a file exists
 *
 * @param filename file to check
 *
 * @return true if exists
 */
int file_exists (char * fileName)
{
    struct stat buf;
    int i = stat ( fileName, &buf );
    /* File found */
    if ( i == 0 ) {
        return 1;
    }
    return 0;
}

/**
 * Send a message without any animation
 *
 * @param matrix  the matrix to present the message
 * @param message the message to display
 */
void quickMessage(LEDMATRIX* matrix, char* message)
{
    // alive message
    matrixCleanModel(matrix);
    matrixPushString(matrix, message);
    matrixSendModel(matrix);
    matrixSendViewport(matrix);
}

/**
 * Read a command from the pipe
 *
 * @param pipe Pipe to read
 * @param data Data passed from the command
 * @param len  Length of data string
 *
 * @return command Identifier
 */
int readCommand(FILE* pipe, char* data, size_t len)
{
    char* dataBuffer;
    int returnCode=COMMAND_BAD;
    char* command = (char*)malloc(sizeof(char)*len);
    dataBuffer = (char*)malloc(DATA_BUFFER_SIZE);
    if (fgets(dataBuffer, DATA_BUFFER_SIZE, pipe)) {
        // delete return carriage
        if (dataBuffer[strlen(dataBuffer)-1] == '\n') {
            dataBuffer[strlen(dataBuffer)-1] = 0;
        }
        // treat the command
        printf("Recieving %s\n", dataBuffer);
        if (commandParse(dataBuffer, command, data, len)==0) {
            printf("  * Command %s\n", command);
            if (strcmp(command, "goodbye")==0) {
                returnCode = COMMAND_QUIT;
            }
            if (strcmp(command, "message")==0) {
                returnCode = COMMAND_MSG;
            }
            if (strcmp(command, "selectFont")==0) {
                returnCode = COMMAND_FONT_SELECT;
            }
        }
        free(dataBuffer);
        free(command);
    }
    return returnCode;
}

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
int commandParse(char* data, char* command, char* argument, size_t len)
{
    int match;
    size_t nMatch;
    size_t commandLen;
    size_t argumentLen;
    int i;
    regex_t preg;
    regmatch_t* matches;
    memset(command, 0, len);
    memset(argument, 0, len);
    //char entry[] = "mycommand    :  et hop";

    if (regcomp(&preg, "^(\\w*)\\s*:(.*)$", REG_EXTENDED)) {
        return 1;
    }

    nMatch = preg.re_nsub+1;
    matches = (regmatch_t*)malloc(sizeof(regmatch_t)*nMatch);

    match = regexec (&preg, data, nMatch, matches, 0);
    regfree(&preg);

    if (!match) {
        commandLen = matches[1].rm_eo - matches[1].rm_so;
        argumentLen = matches[2].rm_eo - matches[2].rm_so;
        memcpy(command, &data[matches[1].rm_so], (commandLen < len) ? commandLen : len);
        memcpy(argument, &data[matches[2].rm_so], (argumentLen < len) ? argumentLen : len);
    }

    free(matches);

    return match;
}

/**
 * Daemon main loop
 *
 * @param matrix  the matrix !
 * @param plugins plugins collection
 * @param fonts   fonts collection
 *
 * @return status
 */
int mainLoop(LEDMATRIX* matrix, ANIMATIONPLUGIN** plugins, FONT** fonts)
{
    char* dataBuffer;
    FILE** fifoFiles;
    ANIMATIONPLUGIN* plugAnimation;
    void* userData = malloc(sizeof(void*));
    ANIMATION_QUEUE* animations=0;
    unsigned int fontSelector=0;
    int command;

    exitCondition = 1;

    // Animation in action
    animations = createAnimationQueue();

    // getting animation from plugin
    plugAnimation = getPluginAnimation(plugins, "scrollV");
    if (plugAnimation) {
        enqueueAnimation(
            animations,
            createAnimation(
                plugAnimation->runtime,
                8,
                0,
                1,
                150,
                userData,
                plugAnimation->creation,
                plugAnimation->destruction
            )
        );
    }

    // prepare the buffer
    dataBuffer = (char*)malloc(DATA_BUFFER_SIZE);

    // Open the communication pipe
    fifoFiles = openCommunicationPipe();

    /* Select font */
    selectFont(matrix, fonts, fontSelector);

    // alive message
    quickMessage(matrix, "Let's go !");

    // main loop
    while (exitCondition) {
        command = readCommand(fifoFiles[0], dataBuffer, DATA_BUFFER_SIZE);
        switch (command) {
            case COMMAND_MSG:
                //fprintf(fifoFiles[1], "OK\n");
                matrixCleanModel(matrix);
                // send the message to the matrix model
                matrixPushString(matrix, dataBuffer);
                // Animation in action
                animate(matrix, animations);
                // wait for a while
                sleep(5);
                break;
            case COMMAND_QUIT:
                exitCondition = 0;
                //fprintf(fifoFiles[1], "Daemon is down\n");
                break;
            case COMMAND_FONT_SELECT:
                sscanf(dataBuffer, "%d", &fontSelector);
                selectFont(matrix, fonts, fontSelector);
                //fprintf(fifoFiles[1], "OK\n");
                break;
            default: break;
        }
    }

    printf("Exiting\n");

    // destroy the animation structure
    destroyAnimationQueue(animations);

    // close the communication pipe
    closeCommunicationPipe(fifoFiles);

    // Dead message
    quickMessage(matrix, "I'm dead !");

    // release buffer
    free(dataBuffer);
    free(userData);

    return 0;
}
