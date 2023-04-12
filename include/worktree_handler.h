#ifndef WORKTREE_HANDLER_H
#define WORKTREE_HANDLER_H

#include "utilities.h"
#include "file_hash.h"
#define WF_STR_SIZE 500
#define WF_TREE_SIZE 200
typedef struct {
    char* name;
    char* hash;
    int mode;
} WorkFile;

typedef struct {
    WorkFile* tab ;
    int size ; //taille maximale
    int n ;     //dernier indice
} WorkTree;

int getChmod ( const char* path);

void setMode ( int mode , char* path );

WorkFile* createWorkFile(char* name);

//on alloue et renvoie un char* contennant les informations du workfile
char* wfts(WorkFile* wf);

//on alloue et renvoie un WF* a partir du char*
WorkFile* stwf(char* ch);

WorkTree* initWorkTree();

void freeWorkFile(WorkFile* wf);

int inWorkTree(WorkTree* wt, char* name);

int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode);

char* wtts(WorkTree* wt);

//String to work tree
WorkTree* stwt(char* str);

//WorkTree* to File
int wttf(WorkTree* wt, char* file);

//File to WorkTree*
WorkTree* ftwt(char* file);

//libere work tree
void freeWorkTree(WorkTree* wt);

//retourner le hash du fichier temporaire qui contient le
char* blobWorkTree(WorkTree* wt);

char* saveWorkTree(WorkTree* wt, char* path);

void restoreWorkTree(WorkTree* wt, char* path);

#endif