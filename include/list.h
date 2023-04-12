#ifndef LIST_H
#define LIST_H
#define LIST_STR_SIZE 500

#include "utilities.h"

typedef struct cell {
    char* data ;
    struct cell * next ;
} Cell, *List ;

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

void freeList(List* L);

List* filterList(List* L, char* pattern);

void printList(List* L);

List* listdir(char* root_dir);

#endif