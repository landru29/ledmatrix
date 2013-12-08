/**
 * Bibliothèque de fonctions de communication avec le hardware
 * via la bibliothèque WiringPi
 *
 * @file    display.h
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "../config.h"

/**
 * Spécifie les pins de chip select
 *
 * @param id identifiant du chipSelect (0|1|2|3)
 * @param value numéro de pin
 */
void setCs(unsigned char id, unsigned char value);

/**
 * Lit les pins de chip select
 *
 * @param id identifiant du chipSelect (0|1|2|3)
 * @return numéro de pin
 */
int getCs(unsigned char id);

/**
 * Initialisation des matrices
 *
 * @param nbMatrix Nombre de matrices à initialiser
 * @param width    Taille d'une matrice
 * @param height   Hauteur d'une matrice
 *
 * @return init's status
 */
int8_t initDisplay(uint8_t nbMatrix, uint8_t width, uint8_t height);

/**
 * Ecrire toutes les données sur les matrices
 *
 * @param viewport Le buffer des données à écrire
 * @param nbMatrix Nombre de matrices sur lesquelles ecrire
 * @param width    La largeur d'une matrice
 * @param height   La hauteur d'une matrice
 */
void writeMatrix(unsigned char* viewport, uint8_t nbMatrix, uint8_t width, uint8_t height);

/**
 * Envoie les commandes d'initialisation à une matrice
 *
 * @param chip Le numéro de matrice
 * @param cmd  La commande
 */
void sendCommand(uint8_t chip, uint8_t cmd);

/**
 * Permet d'activer ou non le blink sur une matrice
 *
 * @param chip   Le numéro de matrice
 * @param blinky Flag d'activation (0 ou 1)
 */
void displayBlink(uint8_t chip, uint8_t blinky);

/**
 * Permet de régler la luminosité d'une matrice
 *
 * @param chip Le numéro de matrice
 * @param pwm  Le niveau de luminosité (0 - 15)
 */
void setBrightness(uint8_t chip, uint8_t pwm);

/**
 * Décaller un buffer de données de bitOffset bits vers la gauche
 *
 * @param buffer     le tampon de données à décaller
 * @param byteLength la taille du tampon en octets
 * @param bitOffset  le nombre de bit à décaller (<8)
 */
void bitShiftLeft(void* buffer, unsigned int byteLength, unsigned int bitOffset);

/**
 * Ecrit une colonne sur la matrice
 *
 * @param chip    Le numéro de matrice
 * @param column  Données de la colonne
 * @param address Adresse de la colonne
 */
void writeColumn(int chip, unsigned char column, unsigned char address);

#endif

/* vim:set expandtab ai nu ts=4 sw=4:
*/
