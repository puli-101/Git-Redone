#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <assert.h>
#include "file_hash.h"
#define true 1
#define false 0
#define STR_SIZE 300

typedef int bool;

/** Ecrit msg en la couleur color sur le fichier output.
 * Les couleurs disponibles sont  "red","yellow","green" et "blue".
 */
void print_color(FILE* output, char* msg, char* color);

/** Renvoie 1 si les deux chaines de caracteres sont identiques. */
int equals(char* str1, char* str2);

/**
 * Fonction qui a partir d'un objet quelconque et une fonction qui enregistre cet objet dans un fichier
 * cree une copie d'un instantanee associe au contenu de l'objet et retourne le hash du fichier
 * utilisee dans blobWorkTree et blobCommit
*/
char* blobContent(void* obj, char* extension, void (*toFile)(void*, char*));

/** Renvoie  1 si path est un fichier normal, 0 si non.*/
int is_regular_file(const char *path);

/** Renvoie 1 si path est un repertoire*/
int is_folder(const char *path);

/** Si le fichier path n'existe pas la fonction renvoie -1. Si non elle renvoie 1 si 
 * le fichier path est vide et 0 s'il n'est pas vide.
*/
int is_empty(const char* path);

#endif