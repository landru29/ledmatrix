#ifndef __PERSO_H
#define __PERSO_H

// http://www.dafont.com/ai-pointe.font

char perso_mapping[] =
	"!\"#$\%&'()*+'-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~";

unsigned int perso_info[] = {
  2,	// !
  5,	// "
  5,	// #
  5,	// $
  5,	// %
  5,	// &
  3,	// '
  4,	// (
  4,	// )
  4,	// *
  5,	// +
  3,	// '
  5,	// -
  3,	// .
  5,	// /
  5,	// 0
  3,	// 1
  5,	// 2
  5,	// 3
  5,	// 4
  5,	// 5
  5,	// 6
  5,	// 7
  5,	// 8
  5,	// 9
  2,	// :
  3,	// ;
  5,	// <
  5,	// =
  5,	// >
  5,	// ?
  5,	// @
  5,	// A
  5,	// B
  5,	// C
  5,	// D
  5,	// E
  5,	// F
  5,	// G
  5,	// H
  3,	// I
  5,	// J
  5,	// K
  5,	// L
  5,	// M
  5,	// N
  5,	// O
  5,	// P
  5,	// Q
  5,	// R
  5,	// S
  5,	// T
  5,	// U
  5,	// V
  5,	// W
  5,	// X
  5,	// Y
  5,	// Z
  3,	// [
  1, 	//
  3,	// ]
  5,	// ^
  5,	// _
  3,	// `
  5,	// a
  5,	// b
  5,	// c
  5,	// d
  5,	// e
  5,	// f
  5,	// g
  5,	// h
  3,	// i
  4,	// j
  5,	// k
  3,	// l
  5,	// m
  4,	// n
  5,	// o
  5,	// p
  5,	// q
  5,	// r
  5,	// s
  5,	// t
  5,	// u
  5,	// v
  5,	// w
  5,	// x
  5,	// y
  5,	// z
  4,	// {
  1,	// |
  8,	// }
  8		// ~
};

unsigned char perso_font[] = {
  0b00111101,	// !
  0b01000000,
  0b00001000,	// "
  0b00010000,
  0b01101000,
  0b00010000,
  0b01100000,
  0b00100010,	// #
  0b01111111,
  0b00100010,
  0b01111111,
  0b00100010,
  0b00110010,	// $
  0b01111111,
  0b01001001,
  0b01111111,
  0b00100110,
  0b01100011,	// %
  0b01100100,
  0b00001000,
  0b00010011,
  0b01100011,
  0b00100110,	// &
  0b01001001,
  0b01010001,
  0b00100010,
  0b00000101,
  0b00001000,	// '
  0b00010000,
  0b01100000,
  0b00011100,	// (
  0b00100010,
  0b01000001,
  0b01000001,
  0b01000001,	// )
  0b01000001,
  0b00100010,
  0b00011100,
  0b01001000,	// *
  0b00110000,
  0b00110000,
  0b01001000,
  0b00001000,	// +
  0b00001000,
  0b00111110,
  0b00001000,
  0b00001000,
  0b00001001,	// ,
  0b00001010,
  0b00000100,
  0b00001000,	// -
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00000111,	// .
  0b00000111,
  0b00000111,
  0b00000011,	// /
  0b00000100,
  0b00001000,
  0b00010000,
  0b01100000,
  0b00111110,	// 0
  0b01000101,
  0b01001001,
  0b01010001,
  0b00111110,
  0b00100001,	// 1
  0b01111111,
  0b00000001,
  0b00110001,	// 2
  0b01000011,
  0b01000101,
  0b01001001,
  0b00110001,
  0b00000110,	// 3
  0b01000001,
  0b01010001,
  0b01010001,
  0b00101110,
  0b00001000,	// 4
  0b00011000,
  0b00101000,
  0b01111111,
  0b00001000,
  0b00010010,	// 5
  0b00110001,
  0b01010001,
  0b01010001,
  0b01001110,
  0b00011110,	// 6
  0b00101001,
  0b01010001,
  0b00010001,
  0b00001110,
  0b01000001,	// 7
  0b01000010,
  0b01000100,
  0b01001000,
  0b00110000,
  0b00101110,	// 8
  0b01010001,
  0b01010001,
  0b01010001,
  0b00101110,
  0b00111000,	// 9
  0b01000101,
  0b01000110,
  0b01000100,
  0b00111000,
  0b01100011,	// :
  0b01100011,
  0b01101001,	// ;
  0b01101010,
  0b00000100,
  0b00001000,	// <
  0b00010100,
  0b00100010,
  0b01000001,
  0b01000001,
  0b00010100,	// =
  0b00010100,
  0b00010100,
  0b00010100,
  0b00010100,
  0b01000001,	// >
  0b01000001,
  0b00100010,
  0b00010100,
  0b00001000,
  0b00100000,	// ?
  0b01000000,
  0b01000011,
  0b01000100,
  0b00111000,
  0b00011110,	// @
  0b00111101,
  0b01100011,
  0b01011101,
  0b00111001,
  0b00011111,	// A
  0b00100100,
  0b01000100,
  0b01000100,
  0b00111111,
  0b01111111,	// B
  0b01001001,
  0b01001001,
  0b00111001,
  0b00000110,
  0b00111110,	// C
  0b01000001,
  0b01000001,
  0b01000001,
  0b00100010,
  0b01111111,	// D
  0b01000001,
  0b01000001,
  0b01000001,
  0b00111110,
  0b00111110,	// E
  0b01001001,
  0b01001001,
  0b01001001,
  0b01001001,
  0b00111111,	// F
  0b01001000,
  0b01001000,
  0b01001000,
  0b01000000,
  0b00111110,	// G
  0b01000001,
  0b01000001,
  0b01001001,
  0b00100110,
  0b01111111,	// H
  0b00001000,
  0b00001000,
  0b00001000,
  0b01111111,
  0b01000001,	// I
  0b01111111,
  0b00000001,
  0b00000110,	// J
  0b00000001,
  0b00000001,
  0b00000001,
  0b01111110,
  0b01111111,	// K
  0b00001000,
  0b00010100,
  0b00100010,
  0b01000001,
  0b01111110,	// L
  0b00000001,
  0b00000001,
  0b00000001,
  0b00000001,
  0b01111111,	// M
  0b00100000,
  0b00011000,
  0b00100000,
  0b01111111,
  0b01111111,	// N
  0b00100000,
  0b00010000,
  0b00001000,
  0b01111111,
  0b00111110,	// O
  0b01000001,
  0b01000001,
  0b01000001,
  0b00111110,
  0b00111111,	// P
  0b01001000,
  0b01000100,
  0b01000100,
  0b00111000,
  0b00111110,	// Q
  0b01000001,
  0b01000001,
  0b01000010,
  0b00111101,
  0b00111111,	// R
  0b01001000,
  0b01000100,
  0b01000110,
  0b00111001,
  0b00110010,	// S
  0b01001001,
  0b01001001,
  0b01001001,
  0b00100110,
  0b01000000,	// T
  0b01000000,
  0b01111111,
  0b01000001,
  0b01000000,
  0b01111110,	// U
  0b00000001,
  0b00000001,
  0b00000001,
  0b01111110,
  0b01111100,	// V
  0b00000010,
  0b00000001,
  0b00000010,
  0b01111100,
  0b01111110,	// W
  0b00000001,
  0b01111110,
  0b00000001,
  0b01111110,
  0b01100011,	// X
  0b00010100,
  0b00001000,
  0b00010100,
  0b01100011,
  0b01110000,	// Y
  0b00001000,
  0b00001111,
  0b00001000,
  0b01110000,
  0b01000010,	// Z
  0b01000101,
  0b01001001,
  0b01010001,
  0b00100001,
  0b01111111,	// [
  0b01000001,
  0b01000001,
  0b00000000, //
  0b01000001,	// ]
  0b01000001,
  0b01111111,
  0b00010000,	// ^
  0b00100000,
  0b01000000,
  0b00100000,
  0b00010000,
  0b00000001,	// _
  0b00000001,
  0b00000001,
  0b00000001,
  0b00000001,
  0b01100000,	// `
  0b00010000,
  0b00001000,
  0b00010110, // a
  0b00101001,
  0b00101001,
  0b00101010,
  0b00011111,
  0b01111110,	// b
  0b00001001,
  0b00010001,
  0b00010001,
  0b00001110,
  0b00011110,	// c
  0b00100001,
  0b00100001,
  0b00100001,
  0b00010010,
  0b00001110,	// d
  0b00010001,
  0b00010001,
  0b00001001,
  0b01111110,
  0b00011110,	// e
  0b00100101,
  0b00100101,
  0b00100101,
  0b00011001,
  0b00000001,	// f
  0b00111111,
  0b01001001,
  0b01001000,
  0b00100000,
  0b00111010,	// g
  0b01000101,
  0b01000101,
  0b01001001,
  0b00111110,
  0b01111111,	// h
  0b00001000,
  0b00010000,
  0b00010000,
  0b00001111,
  0b00010001,	// i
  0b01011111,
  0b00000001,
  0b00000010,	// j
  0b00000001,
  0b00000001,
  0b01011110,
  0b01111111,	// k
  0b00001000,
  0b00001100,
  0b00010010,
  0b00010001,
  0b01111110,	// l
  0b00000001,
  0b00000001,
  0b00111111,	// m
  0b00100000,
  0b00011111,
  0b00100000,
  0b00011111,
  0b00111111,	// n
  0b00100000,
  0b00100000,
  0b00011111,
  0b00011110,	// o
  0b00100001,
  0b00100001,
  0b00100001,
  0b00011110,
  0b00011111,	// p
  0b00100100,
  0b00100100,
  0b00100100,
  0b00011000,
  0b00011100,	// q
  0b00100010,
  0b00100010,
  0b00100100,
  0b00011111,
  0b00111111,	// r
  0b00010000,
  0b00100000,
  0b00100000,
  0b00010000,
  0b00010010,	// s
  0b00101001,
  0b00101001,
  0b00101001,
  0b00000110,
  0b00100000,	// t
  0b01111110,
  0b00100001,
  0b00100001,
  0b00000010,
  0b00111110,	// u
  0b00000001,
  0b00000001,
  0b00000010,
  0b00111111,
  0b00111100,	// v
  0b00000010,
  0b00000001,
  0b00000010,
  0b00111100,
  0b00111110,	// w
  0b00000001,
  0b00111110,
  0b00000001,
  0b00111110,
  0b00010001,	// x
  0b00001010,
  0b00000100,
  0b00001010,
  0b00010001,
  0b00111010,	// y
  0b00000101,
  0b00000101,
  0b00001001,
  0b00111110,
  0b00010001,	// z
  0b00010011,
  0b00010101,
  0b00011001,
  0b00010001,
  0b00001000,	// {
  0b00111110,
  0b01000001,
  0b01000001,
  0b01111111,	// |
  0b00111001, // }
  0b00001001,
  0b11101111,
  0b00111100,
  0b00111100,
  0b11101111,
  0b00001001,
  0b00111001,
  0b00110000, // ~
  0b01001000,
  0b01000100,
  0b00100010,
  0b00100010,
  0b01000100,
  0b01001000,
  0b00110000
};

#endif
