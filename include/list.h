#ifndef LIST_H
#define LIST_H
#define LIST_STR_SIZE 1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
typedef struct cell {
    char* data ;
    struct cell * next ;
} Cell ;

typedef Cell* List ;

List* initList();

Cell* buildCell(char* ch);

void insertFirst(List *L, Cell* C);

char* ctos(Cell* c);

char* ltos(List* L);

Cell* listGet(List* L, int i);

Cell* searchList(List* L, char* str);

List* stol(char* s);

void ltof(List* L, char* path);

List* ftol(char* path);

List* listdir(char* root_dir);

int file_exists(char *file);

void cp(char *to, char *from);

char* hashToPath(char* hash);

#endif