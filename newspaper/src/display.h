#ifndef __DISPLAY_H
#define __DISPLAY_H

/**
 * Ecrire toutes les données sur les matrices
 *
 * @param uint8_t *viewport Le buffer des données à écrire
 * @param uint8_t nbMatrix  Nombre de matrices sur lesquelles ecrire
 * @param uint8_t width     La largeur d'une matrice
 * @param uint8_t height    La hauteur d'une matrice
 *
 * @return void
 */
void writeMatrix(unsigned char* viewport, uint8_t nbMatrix, uint8_t width, uint8_t height);

/**
 * Envoie les commandes d'initialisation à une matrice
 *
 * @param uint8_t chip Le numéro de matrice
 * @param uint8_t cmd  La commande
 *
 * @return void
 */
void sendCommand(uint8_t chip, uint8_t cmd);

/**
 * Permet d'activer ou non le blink sur une matrice
 *
 * @param uint8_t chip   Le numéro de matrice
 * @param uint8_t blinky Flag d'activation (0 ou 1)
 *
 * @return void
 */
void displayBlink(uint8_t chip, uint8_t blinky);

/**
 * Permet de régler la luminosité d'une matrice
 *
 * @param uint8_t chip Le numéro de matrice
 * @param uint8_t pwm  Le niveau de luminosité (0 - 15)
 *
 * @return void
 */
void setBrightness(uint8_t chip, uint8_t pwm);

/**
 * Initialisation des matrices
 *
 * @param uint8_t nbMatrix Nombre de matrices à initialiser
 * @param uint8_t width    Taille d'une matrice
 * @param uint8_t height   Hauteur d'une matrice
 *
 * @return uint8_t
 */
int8_t initDisplay(uint8_t nbMatrix, uint8_t width, uint8_t height);

#endif

/* vim:set expandtab ai nu ts=4 sw=4:
*/
