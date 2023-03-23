#include "commit_handler.h"

kvp* createKeyVal(char* key, char* val) {
    kvp* kv = (kvp*)malloc(sizeof(kvp));
    kv->key = strdup(key);
    kv->value = strdup(val);
    return kv;
}

void freeKeyVal(kvp* kv) {
    free(kv->key);
    free(kv->value);
    free(kv);
}

char* kvts(kvp* k) {
    char* str = (char*)malloc(sizeof(char) * (COMMIT_STR / 10));
    //format : key : value
    sprintf(str,"%s : %s", k->key, k->value);
    return str;
}

kvp* stkv(char* str) {
    char key[(COMMIT_STR / 10)];
    char val[(COMMIT_STR / 10)];
    kvp* kv;

    sscanf(str," %s : %s", key, val);
    kv = createKeyVal(key,val);

    return kv;
}

Commit* initCommit() {
    Commit* c = (Commit*)malloc(sizeof(Commit));
    c->n = MAX_COMMIT;
    c->size = 0;
    c->T = (kvp**)malloc(sizeof(kvp*) * MAX_COMMIT);
    assert(c->T != NULL);
    for (int i = 0; i < MAX_COMMIT; i++) {
        c->T[i] = NULL;
    }
    return c;
}

void commitSet(Commit* c, char* key, char* value) {
    if (c->size >= c->n) return;
    unsigned long index = hash((unsigned char*)key) % MAX_COMMIT;
    int i = (index + 1) % MAX_COMMIT;   //position suivante

    //si la position index est libre...
    if (c->T[index] == NULL) {
        c->T[index] = createKeyVal(key, value);
        c->size++;
        return;
    }
    
    //sinon on teste toutes les autres positions avec probing lineaire
    while (i != index) {
        if (c->T[i] == NULL) {
            c->T[i] = createKeyVal(key, value);
            c->size++;
            return;
        }
        i++;
        i %= MAX_COMMIT;
    }

    
}

//hash 33
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}

Commit* createCommit(char* hash) {
    Commit* c = initCommit();
    commitSet(c,"tree", hash);
    return c;
}

/** 
 * On recupere la valeur associe a key
 * On suppose qu'on ne peut pas supprimer des elements du tableau
 * alors la premiere case vide qu'on retrouve lors du parcours implique 
 * que l'element de cle key n'est pas present
*/
char* commitGet(Commit* c, char* key) {
    int index = hash((unsigned char*)key) % MAX_COMMIT, i = (index + 1) % MAX_COMMIT;
    //on suppose qu'on ne supprime pas les commits (la table de hachage n'est pas dynamique)
    if (c->T[index] == NULL) {
        return NULL;
    } else if (strcmp(c->T[index]->key, key) == 0) {
        return c->T[index]->value;
    }

    //parcours du probing cyclique lineaire
    while (i != index) {
        if (c->T[i] == NULL) {
            return NULL;
        } else if (strcmp(c->T[i]->key, key) == 0) {
            return c->T[i]->value;
        }
        i++;
        i %= MAX_COMMIT;
    }

    return NULL;
}

//Commit to string
char* cts(Commit* c) {
    char* str = (char*)malloc(sizeof(char) * COMMIT_STR);
    char* kv_str;

    str[0] = '\0';
    for (int i = 0; i < c->n; i++) {
        if (c->T[i] != NULL) {
            kv_str = kvts(c->T[i]);
            strcat(str,kv_str);
            strcat(str,"\n");
            free(kv_str);
        }
    }
    return str;
}

//on decale la tete de la chaine associee a **buff et on itere lorseque la ligne n'est pas fini
//on recopie les valeurs dans line
char* extractLine(char* line, char** buff) {
    int i;
    for (i = 0; **buff != '\0' && **buff != '\n'; (*buff)++, i++) {
        line[i] = **buff;
    }
    if (i == 0)
        return NULL;
    
    if (**buff != '\0')
        (*buff)++;
    line[i] = '\0';
    return *buff;
}

Commit* stc(char* ch) {
    Commit* c = initCommit();
    char ligne[COMMIT_STR];
    char key[COMMIT_STR];
    char val[COMMIT_STR];
    
    while(extractLine(ligne, &ch)) {
        sscanf(ligne, " %s : %s", key, val);
        commitSet(c, key, val);
    }
    
    return c; 
}

//Commit to File
void ctf(Commit* c, char* file) {
    FILE* f = fopen(file, "w");
    char* str = cts(c);
    fprintf(f,"%s",str);
    free(str);
    fclose(f);
}

//File to commit
Commit* ftc(char* file) {
    FILE* f = fopen(file, "r");
    char ligne[COMMIT_STR], key[COMMIT_STR], value[COMMIT_STR];
    Commit* c = initCommit();

    //ligne par ligne...
    while(fgets(ligne, COMMIT_STR, f)) {
        //on extrait key et value et on l'insere dans la table d'hachage
        sscanf(ligne, " %s : %s\n",key, value);
        commitSet(c, key, value);
    }

    fclose(f);
    return c;
}

//fonction temporaire qui fait le cast de void* vers Commit* pour pouvoir utiliser ctf
void castCommitToFile(void * obj, char* file) {
    ctf((Commit*)obj, file);
}

char* blobCommit(Commit* c) {
    return blobContent((void*) c, ".c", castCommitToFile);
}

void freeCommit(Commit* c) {
    for (int i = 0; i < c->n ; i++) {
        if (c->T[i] != NULL) {
            free(c->T[i]->key);
            free(c->T[i]->value);
            free(c->T[i]);
        }
    }
    free(c->T);
    free(c);
}