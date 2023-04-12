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
void print_color(FILE* output, char* msg, char* color);

int equals(char* str1, char* str2);

/**
 * Fonction qui a partir d'un objet quelconque et une fonction qui enregistre cet objet dans un fichier
 * cree une copie d'un instantanee associe au contenu de l'objet et retourne le hash du fichier
 * utilisee dans blobWorkTree et blobCommit
*/
char* blobContent(void* obj, char* extension, void (*toFile)(void*, char*));

int is_regular_file(const char *path);

int is_folder(const char *path);

int is_empty(const char* path);

#endif