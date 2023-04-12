#ifndef COMMIT_HANDLER_H
#define COMMIT_HANDLER_H

#include "list.h"
#include "file_hash.h"
#include "worktree_handler.h"
#include "utilities.h"

#define MAX_COMMIT 1000
#define COMMIT_STR 10000
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

kvp* createKeyVal(char* key, char* val);

void freeKeyVal(kvp* kv);

char* kvts(kvp* k);

kvp* stkv(char* str);

Commit* initCommit();

unsigned long hash(unsigned char *str);

Commit* createCommit(char* hash);

void commitSet(Commit* c, char* key, char* value);

char* commitGet(Commit* c, char* key);

Commit* stc(char* ch);

void ctf(Commit* c, char* file);

void freeCommit(Commit* c);

Commit* ftc(char* file);

char* cts(Commit* c);

char* blobCommit(Commit* c);

void restoreCommit(char* hash_commit);

#endif