#include "commit_handler.h"

/** Alloue et fabrique la kvp correpondant aux données saisies en arguments*/
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

/** kvp to string: alloue et renvoie la chaine de charactères correspendant à la kvp */
char* kvts(kvp* k) {
    char* str = (char*)malloc(sizeof(char) * (COMMIT_STR / 10));
    //format : key : value
    sprintf(str,"%s : %s", k->key, k->value);
    return str;
}

/** String to kvp: alloue et renvoie un poointeur sur la kvp decrite par str. */
kvp* stkv(char* str) {
    char key[(COMMIT_STR / 10)];
    char val[(COMMIT_STR / 10)];
    kvp* kv;

    sscanf(str," %s : %s", key, val);
    kv = createKeyVal(key,val);

    return kv;
}

/** Alloue et renvoie un pointeur su un nouveau commit.*/
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

/** Ajoute une kvp à la table d'hachage du commit c si possible. */
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

/** Renvoie le hash 33 de str.*/
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}

/** Renvoie un pointeur sur un nouveau commit. La table d'hachage du nouveau
 * commit contiendra une kvp de valeur ("tree" : hash).
*/
Commit* createCommit(char* hash) {
    Commit* c = initCommit();
    commitSet(c,"tree", hash);
    return c;
}

/** On recupere la valeur associée à key dans le commit c. */
char* commitGet(Commit* c, char* key) {
    int index = hash((unsigned char*)key) % MAX_COMMIT, i = (index + 1) % MAX_COMMIT;
    //on suppose qu'on ne supprime pas les commits (la table de hachage n'est pas dynamique)
    if (c->T[index] == NULL) {
        return NULL;
    } else if (strcmp(c->T[index]->key, key) == 0) {
        return c->T[index]->value;
    }

    //parcours du probing cyclique lineaire
    //On suppose qu'on ne peut pas supprimer des éléments du tableau. 
    while (i != index) { 
        if (c->T[i] == NULL) {
            return NULL; 
            //si on trouve une casse vide et on n'a pas trouvé la kvp cherchee, elle n'existe pas
        } else if (strcmp(c->T[i]->key, key) == 0) {
            return c->T[i]->value;
        }
        i++;
        i %= MAX_COMMIT;
    }

    return NULL;
}

/** Commit to string: renvoie un pointeur sur la chaine de caractères representant un commit 
 * (que la fonction alloue) */
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
/** copie sur line la premiere partie de buff representant une ligne*/
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

/** string to commit: Renvoie un pointeur sur la chaine de caracteres decrivant un commit*/
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

/** Commit to File: cree le fichier file contenant la chaine de caracteres representant le commit c */
void ctf(Commit* c, char* file) {
    FILE* f = fopen(file, "w");
    char* str = cts(c);
    fprintf(f,"%s",str);
    free(str);
    fclose(f);
}

/** Alloue et renvoie un pointeur sur le commit decrit par le contenu du fichier file. */
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

/** fonction auxiliaire qui fait le cast de void* vers Commit* pour pouvoir utiliser ctf */
void castCommitToFile(void * obj, char* file) {
    ctf((Commit*)obj, file);
}

/** Cree  le blob (copie instantanée) du commit et renvoie le hash du blob */
char* blobCommit(Commit* c) {
    return blobContent((void*) c, ".c", castCommitToFile);
}

/** Libere tout l'espace memoire employe par le commit c. */
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

/** La fonction va chercher le worktree du commit de hash commit et le restaurer. */
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