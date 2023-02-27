#ifndef FILE_HASH_H
#define FILE_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

int hashFile(char* source, char* dest);

char* sha256file(char* file);

#endif