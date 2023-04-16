#ifndef LIST_H
#define LIST_H
#define LIST_STR_SIZE 500

#include "utilities.h"

typedef struct cell {
    char* data ;
    struct cell * next ;
} Cell, *List ;

/** Renvoie un pointeur sur un espace de la memoire alloue pour une Liste qui est initialisee 
 * avec la valeur NULL.
 */
List* initList();

/**
 * Alloue l'espace memoire pour une cellule contenant dans data la chaine ch.
 */
Cell* buildCell(char* ch);

/**
 * Insertion de C dans la tête de la liste L.
 */
void insertFirst(List *L, Cell* C);

/**
 * Renvoie la chaine de characteres stockee dans la cellule c.
 */
char* ctos(Cell* c);

/** 
 * Renvoie la concatenation des informations d'une liste, les éléments sont
 *separes par "|" 
 */
char* ltos(List* L);

/**
 * Renvoie un pointeur sur la cellule d'indice i de la liste L.
 */
Cell* listGet(List* L, int i);

/**
 * Renvoie un pointeur sur la premiere chaine de la liste ayant pour data la chaine str si cette cellule existe.
 * Sinon NULL est renvoye.
 */
Cell* searchList(List* L, char* str);

/**
 * Reconstruit une liste à partir d'une chaine de characteres.
 */
List* stol(char* s);

/**
*list-to-file: Ecrit sur le fichier path la chaine de characteres representant la liste. 
*/
void ltof(List* L, char* path);

/** 
 * File-to-list: Lit un fichier et renvoie un pointeur sur une liste qui contient son contenu 
 */
List* ftol(char* path);

/**
 * Libere tout l'espace pris par la Liste L et son contenu.
 */
void freeList(List* L);

/**
 * Copie et fait une liste des elements de L qui ont pour prefixe pattern.
 */
List* filterList(List* L, char* pattern);

/**
 * Affiche le contenu d'une Liste.
 */
void printList(List* L);

/**
 * Renvoie une liste ou chaque cellule contient le nom d'un element du repertoire.
 */
List* listdir(char* root_dir);

#endif