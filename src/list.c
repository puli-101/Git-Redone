#include "list.h"

/** Renvoie un pointeur sur un espace de la memoire alloue pour une Liste qui est initialisee 
 * avec la valeur NULL.
 */
List* initList() {
    List* l = (List*)malloc(sizeof(List));
    *l = NULL;
    return l;
}

/**
 * Alloue l'espace memoire pour une cellule contenant dans data la chaine ch.
 */
Cell* buildCell(char* ch) {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    cell->data = strdup(ch);
    cell->next = NULL;
	return cell;
}

/**
 * Insertion de C dans la tête de la liste L.
 */
void insertFirst(List *L, Cell* C) {
    if (L == NULL) return;
    C->next = *L;
    *L = C;
}

/**
 * Renvoie la chaine de characteres stockee dans la cellule c.
 */
char* ctos(Cell* c) {
    return c->data;
}

/** 
 * Renvoie la concatenation des informations d'une liste, les éléments sont
 *separes par "|" 
 */
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

/**
 * Renvoie un pointeur sur la cellule d'indice i de la liste L.
 */
Cell* listGet(List* L, int i) {
    Cell* tmp = *L;
    int j = 0;
    while(tmp && j < i) {
        j++;
        tmp = tmp->next;
    }
    return tmp;
}

/**
 * Renvoie un pointeur sur la premiere chaine de la liste ayant pour data la chaine str si cette cellule existe.
 * Sinon NULL est renvoye.
 */
Cell* searchList(List* L, char* str) {
    Cell* tmp = *L;
    while(tmp) {
        if (!strcmp(tmp->data,str))
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

/**
 * Reconstruit une liste à partir d'une chaine de characteres.
 */
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

/**
*list-to-file: Ecrit sur le fichier path la chaine de characteres representant la liste. 
*/
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

/** 
 * File-to-list: Lit un fichier et renvoie un pointeur sur une liste qui contient son contenu 
 */
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

/**
 * Libere tout l'espace pris par la Liste L et son contenu.
 */
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

/**
 * Affiche le contenu d'une Liste. 
 */
void printList(List* L) {
    char* str = ltos(L);
    printf("%s\n",str);
    free(str);
}

/**
 * La fonction renvoie 1 si le paramètre prefix est un prefixe du paramètre word. Sinon elle renvoie 0.
 */
int isPrefix(char* prefix, char* word) {
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (word[i] == '\0' || prefix[i] != word[i])
            return 0;
    }
    return 1;
}

/**
 * Copie et fait une liste des elements de L qui ont pour prefixe pattern.
 */
List* filterList(List* L, char* pattern) {
    List* l = initList();
    for (Cell* e = *L; e != NULL; e = e->next) {
        char* str = e->data;
        if (isPrefix(pattern, str)) {
            insertFirst(l, buildCell(str));
        }
    }
    return l;
}

/**
 * Renvoie une liste ou chaque cellule contient le nom d'un element du repertoire.
 */
List* listdir(char* root_dir) {
    DIR * dp = opendir ( root_dir ) ;
    List* l = initList();

    struct dirent * ep ;
    if ( dp != NULL ) {
        while ((ep = readdir (dp)) != NULL ) {
            insertFirst(l, buildCell(ep->d_name));
        }
    }

    if (dp != NULL)
        closedir(dp);

    return l;
}