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

/** Renvoie le mode du document d'adresse path.*/
int getChmod ( const char* path);

/** Modifie à @mode le mode du fichier d'adresse path.*/
void setMode ( int mode , char* path );

/** Allocation et initialisation d'un workfile à partir de la chaine name. */
WorkFile* createWorkFile(char* name);

/** Workfile to string: On alloue et retourne une chaine de charactères représentant un workfile */
char* wfts(WorkFile* wf);

/** On alloue et renvoie un WF* a partir du char. **/
WorkFile* stwf(char* ch);

/** Alloue et renvoie un pointeur d'un nouveau worktree dont le tableau de WorkFiles est vide. */
WorkTree* initWorkTree();

/** libere worktree */
void freeWorkFile(WorkFile* wf);

/** La fonction cherche un workfile correspondant au fichier de nom char dans le worktree et elle 
 * renvoie son indice dans le tableau du worktree. Si le fichier est absent un -1 est renvoye. */
int inWorkTree(WorkTree* wt, char* name);

/** S'il reste d'espace, elle ajoute le workfile contenant l'information des parametres au 
 * worktree wt. La fonction renvoie l'indice de la caisse ou le nouveau workfile est stocke, 
 * cet indice est -1 s'il ne reste plus d'espace.*/
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode);

/** worktree to string: renvoie la chaine de caracteres associee a wt */
char* wtts(WorkTree* wt);

/** String to worktree: renvoie le worktree represente par str. */
WorkTree* stwt(char* str);

/** WorkTree to File: Cree une File file contenant la chaine de caracteres decrivant wt*/
int wttf(WorkTree* wt, char* file);

/** File to WorkTree: on suppose que chaque ligne a le bon format. Renvoie un pointeur sur un worktree
 * construit a partir du contenu de file. */
WorkTree* ftwt(char* file);

/** libere worktree */
void freeWorkTree(WorkTree* wt);

/** retourne le hash du fichier temporaire qui contient le string du worktree wt. */
char* blobWorkTree(WorkTree* wt);

/** Fait un blob (copie instantanee) de tous les fichiers dans work tree et stocke leur hash dans la workfile
 *de chaque fichier. Puis un blob du worktree est effectué et le hash de ce dernier blob est retourné. */
char* saveWorkTree(WorkTree* wt, char* path);

/** La fonction va chercher le fichier temporaire du contenu enregistré par le worktree
 * La fonction fait le transfert des copies d'une instance des fichiers aux fichiers originaux.
 */
void restoreWorkTree(WorkTree* wt, char* path);

#endif