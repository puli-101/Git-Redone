#ifndef REFERENCE_HANDLER_H
#define REFERENCE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void initRefs();

void createUpdateRef(char* ref_name, char* hash);

void deleteRef(char* ref_name);

char* getRef(char* ref_name);

#endif