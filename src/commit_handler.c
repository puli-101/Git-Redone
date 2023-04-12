#include "commit_handler.h"
/**
 * Alloue l'espace pour un kvp et crée la kvp correpondant aux données saisies en arguments
 */
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

/*kvp to string: alloue et renvoie la chaine de charactères correspendant à la kvp*/
char* kvts(kvp* k) {
    char* str = (char*)malloc(sizeof(char) * (COMMIT_STR / 10));
    //format : key : value
    sprintf(str,"%s : %s", k->key, k->value);
    return str;
}

/*String to kvp:*/
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

/*Ajoute une kvp à la table d'hachage du commit c si possible.
Les collisions de notre table d'hachage sont gerées par 
adressage ouvert linéaire.*/
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
/**
 * Fonction d'hachage choisie:
 */
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}

/**Initilialise et alloue un nouveau commit pour renvoyer un pointeur de ce commit
 * La table d'hachage du commit contiendra une kvp de valeur ("tree" : hash).
*/
Commit* createCommit(char* hash) {
    Commit* c = initCommit();
    commitSet(c,"tree", hash);
    return c;
}

/** 
 * On recupere la valeur associée à key
 * On suppose qu'on ne peut pas supprimer des éléments du tableau
 * alors la premiere case vide qu'on retrouve lors du parcours implique 
 * que l'élément de cle key n'est pas présent
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

/** string to commit:
*/
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

/**
 * Crée  une copie instantanée du commit nomée d'apres le hash du commit et 
 * renvoie le hash du nouveau commit.
 */
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

/**
 * La fonction va chercher le worktree du commit de hash commit et le restaurer.
 */
void restoreCommit(char* hash_commit) {
    char *commit_path = hashToPathExtension(hash_commit, ".c") ;
    Commit *c = ftc (commit_path);

    char* wt_hash = strdup(commitGet (c , "tree"));
    char *wt_path = hashToPathExtension(wt_hash, ".t");

    WorkTree * wt = ftwt (wt_path);

    restoreWorkTree(wt , ".");

    freeCommit(c);
    freeWorkTree(wt);
    free(commit_path);
    free(wt_hash);
    free(wt_path);
}