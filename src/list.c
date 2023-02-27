#include "list.h"

List* initList() {
    List* l = (List*)malloc(sizeof(List));
    *l = NULL;
    return l;
}

Cell* buildCell(char* ch) {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    cell->data = strdup(ch);
    cell->next = NULL;
	return cell;
}

void insertFirst(List *L, Cell* C) {
    if (L == NULL) return;
    C->next = *L;
    *L = C;
}

char* ctos(Cell* c) {
    return c->data;
}

char* ltos(List* L) {
    char* str = malloc(sizeof(char) * LIST_STR_SIZE);
    str[0] = '\0';

    Cell* tmp = *L;
    while(tmp) { 
	    strcat(str,"|");
        strcat(str,ctos(tmp));
        tmp = tmp->next;
    }
    return str;
}

Cell* listGet(List* L, int i) {
    Cell* tmp = *L;
    int j = 0;
    while(tmp && j < i) {
        j++;
        tmp = tmp->next;
    }
    return tmp;
}

Cell* searchList(List* L, char* str) {
    Cell* tmp = *L;
    while(tmp) {
        if (!strcmp(tmp->data,str))
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

List* stol(char* s) {
    char buff[LIST_STR_SIZE];
    int buf_i = 0;
    List* l = initList();
    
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '|') {
            buff[buf_i] = '\0';
            insertFirst(l, buildCell(buff));
            buf_i = 0;
        } else {
            buff[buf_i] = s[i];
            buf_i++;
        }
    }
    //on insere le dernier element
    if (buf_i != 0) {
        buff[buf_i] = '\0';
        insertFirst(l, buildCell(buff));
    }

    return l;
}

void ltof(List* L, char* path) {
    FILE* f = fopen(path, "w");
    char* str = ltos(L);
    fprintf(f, "%s", str);
    free(str);
    fclose(f);
}

List* ftol(char* path) {
    FILE* f = fopen(path, "r");
    char buffer[LIST_STR_SIZE];
    if (!fgets(buffer, LIST_STR_SIZE, f))
        return NULL;
    fclose (f);
    return stol(buffer);
}