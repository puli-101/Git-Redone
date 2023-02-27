#ifndef FILE_HASH_H
#define FILE_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#define HASH_STR_SIZE 500

int hashFile(char* source, char* dest);

char* sha256file(char* file);

List* listdir(char* root_dir);

int file_exists(char *file);

void cp(char *to, char *from);

char* hashToPath(char* hash);

void blobFile(char* file);

#endif