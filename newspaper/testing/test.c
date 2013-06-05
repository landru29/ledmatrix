#include <stdio.h>
#include "font.h"
#include "ledmatrix.h"
#include "aipointe.h"
#include <unistd.h>
#include "animationpool.h"
#include "animate.h"


void basicMatrixTest() {
	LEDMATRIX* matrix=0;
	FONT* font=0;
	LETTER letter;
	char word[] = "taleo";
	char digitaleo[] = "Digitaleo";
	
	printf("* Testing library\n");
	matrix = openLedMatrix(96, 8);
	matrixDebug(matrix);
	
	printf("* Loading font\n");
	font = createFont(aipointe_font, aipoint_info, aipoint_mapping, 1);
	fontDebug(font);
	
	printf("* Linking font to the matrix\n");
	matrixSetFont(matrix, font);
	
	printf("* Picking a letter\n");
	letter = getLetter('d', font);
	letterDebug(letter);
	
	printf("* Adding a letter\n");
	matrixPushLetter(matrix, letter);
	
	printf("* Adding other letters\n");
	letter = getLetter('i', font);
	matrixPushLetter(matrix, letter);
	letter = getLetter('g', font);
	matrixPushLetter(matrix, letter);
	letter = getLetter('i', font);
	matrixPushLetter(matrix, letter);
	
	printf("* Checking model\n");
	matrixDebug(matrix);
	
	printf("* Sending a string\n");
	matrixPushString(matrix, word);
	matrixDebug(matrix);
	
	printf("* Erasing model and sending a string\n");
	matrixCleanModel(matrix);
	matrixPushString(matrix, digitaleo);
	matrixDebug(matrix);
	
	printf("* Closing all\n");
	destroyFont(font);
	closeLedMatrix(matrix);
	printf("* Test OK\n");
}

void animationTest()
{
	ANIMATION* myAnimation=0;
	ANIMATION_QUEUE* animations=0;
	LEDMATRIX* matrix=0;
	printf("* Creating matrix\n");
	matrix = openLedMatrix(96, 8);
	printf("* Testing animation\n");
	myAnimation = createAnimation(testFrame, 0, 10, 1000);
	printf("* Animating\n");
	animateOne(matrix, myAnimation);
	printf("* End of animation\n");
	destroyAnimation(myAnimation);
	
	printf("* Testing animation list\n");
	animations = createAnimationQueue();
	printf("* Pushing animations\n");
	enqueueAnimation(animations, createAnimation(testFrame, 0, 10, 1000));
	enqueueAnimation(animations, createAnimation(testFrame, 9, 0, 1000));
	printf("* Animating\n");
	animate(matrix, animations);
	printf("* Cleaning\n");
	closeLedMatrix(matrix);
}


void usage()
{
	printf("test -a -b -x\n");
}

int main(int argc, char** argv)
{
	char optstring[] = "x:ba";
	int option;
	
	if (argc<2) {
		usage();
		return 0;
	}
	
	opterr=0; /* Pas de message d'erreur automatique */
	
	while ((option = getopt(argc, argv, optstring)) != -1) {
		switch (option) {
			case 'a': /* animation test */
				animationTest();
				break;
			case 'b': /* basics on led matrix */
				basicMatrixTest();
				break;
			case 'x': /* example of options with value */
				printf("This is an example: %s\n", optarg);
				break;
			default:
				usage();
				break;
		}
	}
	
	return 0;
}
