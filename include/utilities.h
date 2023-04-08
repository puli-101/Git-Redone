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

void print_color(FILE* output, char* msg, char* color);
int equals(char* str1, char* str2);

#endif