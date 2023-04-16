#ifndef BRANCH_HANDLER_H
#define BRANCH_HANDLER_H

#include "reference_handler.h"
#include "list.h"
#include "utilities.h"

/** Duplique le master dans le fichier cache current_branch. */
void initBranch();



/** Fait que le head pointe dans la branche indiquee. */
void createBranch(char* branch);

/** Verifie si la branche branch existe dans le repertoire de references renvoie 1 si c'est le cas. */
int branchExists(char* branch);

/**
 *  Affiche ligne par liche les commits d'une branche.
 * Les informations sont presentees sous la forme:
 * Hash: ...
 * Message: ...
 */
void printBranch(char* branch);

/**
 * Initialise et alloue l'espace de memoire pour rendre le pointeur sur une liste contenant
 * dans ces cellules les hash des commits de la branche branch.
 */
List* branchList(char* branch);

/** Alloue un espace de memoire pour garder et renvoyer un pointeur sur une chaine de caracteres ayant 
 * le contenu de .current_branch */
char* getCurrentBranch();

/**
 * Cette fonction renvoie un pointeur sur une liste avec tous les commits de toutes les branches */
List* getAllCommits();

/**
 * La fonction va ecrire le nom de la branche branch sur ".current_branch",
 * elle actualise aussi HEAD pour qu'il pointe sur la nouvelle branche courrante.
 * Un message d'erreur est affiche si la branche branch n'existe pas dans ./refs.
 */
void myGitCheckoutBranch(char* branch);

/**
 * Selon le patron insere en parametres, la fonction va afficher la liste des commits qui 
 * ont pour prefixe pattern dans leur hash. S'il ne reste qu'un commit ayant le patron pour préfixe, la
 * Head va pointer sur ce commit et ce Commit est restaure.
 * Si aucun commit a pour prefixe pattern un message d'erreur est affiché.
 */
void myGitCheckoutCommit(char* pattern);

#endif