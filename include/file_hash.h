#ifndef FILE_HASH_H
#define FILE_HASH_H

#include "list.h"
#define HASH_STR_SIZE 500

//ecrit le hash du fichier source dans le fichier dest
int hashFile(char* source, char* dest);

//on alloue et retourne le hash du fichier file sous forme d'une chaine de caracteres
char* sha256file(char* file);

List* listdir(char* root_dir);

int file_exists(char *file);

void cp(char *to, char *from);

char* hashToPath(char* hash);

void blobFile(char* file);

#endif