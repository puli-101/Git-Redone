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

/*renvoie la concatenation des informations d'une liste, les éléments sont
 séparés par "|" */
char* ltos(List* L) {
    char* str = malloc(sizeof(char) * LIST_STR_SIZE);
    str[0] = '\0';

    Cell* tmp = *L;
    while(tmp) { 
        strcat(str,ctos(tmp));
        strcat(str,"|");
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

/**Reconstruit une liste à partir d'une chaine de charactères */
List* stol(char* s) {
    char buff[LIST_STR_SIZE];
    int buf_i = 0;  //compteur de la taille du mot
    List* l = initList();
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '|') {
            //on ignore les mots vides
            if (buf_i == 0) 
                continue;
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

/*list-to-file: Écrit sur le fichier path la chaine de charactères représentant la liste */
void ltof(List* L, char* path) {
    FILE* f = fopen(path, "w");
    if (f == NULL) {
        fprintf(stderr,"Unable to create %s file\n", path);
        exit(-1);
    }
    char* str = ltos(L);
    fprintf(f, "%s", str);
    free(str);
    fclose(f);
}

/**File-to-list: Lit un fichier et renvoie un pointeur sur une liste qui contient son contenu */
List* ftol(char* path) {
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        fprintf(stderr,"Unable to open %s file\n", path);
        exit(-1);
    }
    char buffer[LIST_STR_SIZE];
    if (!fgets(buffer, LIST_STR_SIZE, f))
        return NULL;
    fclose (f);
    return stol(buffer);
}

void freeList(List* L) {
    if (!L) return;
    Cell* c = *L, *tmp;
    while(c) {
        tmp = c->next;
        free(c->data);
        free(c);
        c = tmp;
    }
    free(L);
}

void printList(List* L) {
    char* str = ltos(L);
    printf("%s\n",str);
    free(str);
}