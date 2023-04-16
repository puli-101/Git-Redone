#ifndef COMMIT_HANDLER_H
#define COMMIT_HANDLER_H

#include "list.h"
#include "file_hash.h"
#include "worktree_handler.h"
#include "utilities.h"

#define MAX_COMMIT 1000
#define COMMIT_STR 1000
typedef struct key_value_pair {
    char * key ;
    char * value ;
} kvp ;

typedef struct hash_table {
    kvp ** T ;
    int n ;
    int size ;
} HashTable ;

typedef HashTable Commit ;

/** Alloue et fabrique la kvp correpondant aux données saisies en arguments*/
kvp* createKeyVal(char* key, char* val);

void freeKeyVal(kvp* kv);

/** kvp to string: alloue et renvoie la chaine de charactères correspendant à la kvp */
char* kvts(kvp* k);

/** String to kvp: alloue et renvoie un poointeur sur la kvp decrite par str. */
kvp* stkv(char* str);

/** Alloue et renvoie un pointeur su un nouveau commit.*/
Commit* initCommit();

/** Renvoie le hash 33 de str.*/
unsigned long hash(unsigned char *str);

/** Renvoie un pointeur sur un nouveau commit. La table d'hachage du nouveau
 * commit contiendra une kvp de valeur ("tree" : hash).
*/
Commit* createCommit(char* hash);

/** Ajoute une kvp à la table d'hachage du commit c si possible. */
void commitSet(Commit* c, char* key, char* value);

/** On recupere la valeur associée à key dans le commit c. */
char* commitGet(Commit* c, char* key);

/** string to commit: Renvoie un pointeur sur la chaine de caracteres decrivant un commit*/
Commit* stc(char* ch);

/** Commit to File: cree le fichier file contenant la chaine de caracteres representant le commit c */
void ctf(Commit* c, char* file);

/** Libere tout l'espace memoire employe par le commit c. */
void freeCommit(Commit* c);

/** Alloue et renvoie un pointeur sur le commit decrit par le contenu du fichier file. */
Commit* ftc(char* file);

/** Commit to string: renvoie un pointeur sur la chaine de caractères representant un commit 
 * (que la fonction alloue) */
char* cts(Commit* c);

/** Cree  le blob (copie instantanée) du commit et renvoie le hash du blob */
char* blobCommit(Commit* c);

/** La fonction va chercher le worktree du commit de hash commit et le restaurer. */
void restoreCommit(char* hash_commit);

/** fonction auxiliaire qui fait le cast de void* vers Commit* pour pouvoir utiliser ctf */
void castCommitToFile(void * obj, char* file);

#endif
