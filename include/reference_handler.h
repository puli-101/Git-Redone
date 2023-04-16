#ifndef REFERENCE_HANDLER_H
#define REFERENCE_HANDLER_H

#include "utilities.h"
#include "commit_handler.h"
#include "worktree_handler.h"

/** Cette fonction va creer le repertoire caché de reférences si il n'existe pas.
 * Dans ce repertoire, les fichiers vides  Head et Master sont crees. */
void initRefs();

/** Le fichier de nom ref_name est ouvert ou cree s'il n'existe pas dans le repertoire de 
 *références. hash est écrit dessus ce fichier. */
void createUpdateRef(char* ref_name, char* hash);

/** Elimine la reference mise en parametres. */
void deleteRef(char* ref_name);

/** Si ref_name existe dans le fichier ./refs, un pointeur su une chaine de caracteres allouee
 * contenant le texte du fichier ref_name est renvoye. */
char* getRef(char* ref_name);

/** Ajoute le fichier file_or_folder au WorkTree decrit par ".add", notre zone de preparation. */
void myGitAdd(char* file_or_folder);

/** Cree le commit pour message @message et pour worktree le worktree de ".add" sur la 
 * branche branch_name. */
void myGitCommit(char* branch_name, char* message);

#endif