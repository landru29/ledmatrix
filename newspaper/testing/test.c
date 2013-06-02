#include <stdio.h>
#include <font.h>
#include <ledmatrix.h>
#include <aipointe.h>





int main()
{
	LEDMATRIX* matrix=0;
	FONT* font=0;
	LETTER letter;
	char word[] = "taleo";
	char digitaleo[] = "Digitaleo";
	
	printf("* Testing library\n");
	matrix = openLedMatrix(32, 8);
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
	
	return 0;
}
