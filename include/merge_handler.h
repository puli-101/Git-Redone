#ifndef MERGE_HANDLER_H
#define MERGE_HANDLER_H

#include "branch_handler.h"
#include "list.h"

/**
 * La fonction cree et renvoie un pointeur sur un worktree contenant les workfiles des fichiers de wt1
 * et wt2 qui ne sont pas en conflit. Les fichiers en conflit sont ajoutes a la liste conflicts.
 */
WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts);

/**
 *  Si il n'y a pas des conflits entre la remote branch et la branche actuelle, 
 * Un worktree wt contenant les fichiers des deux branches est alloue.
 * Un commit  ayant pour worktree  wt est crée et la current branch est actualisée,
 * elle pointe maintenant a ce nouveau commit. 
 * Ce commit aura pour predecesseur le dernier commit de la branche courante et 
 * additionellement il aura pour merged predecessor le dernier commit de remote_branch.
 *HEAD et current branch sont actualisés et wt est restaure.
 */
List* merge(char* remote_branch, char* message);

/**
 * La fonction va creer un commit sur la branche branch ou seulement les documents
 * absents dans la liste conflicts sont enregistres par le worktree du commit.
 */
void createDeletionCommit(char* branch, List* conflicts, char* message);

#endif