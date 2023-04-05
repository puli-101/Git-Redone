#ifndef BRANCH_HANDLER_H
#define BRANCH_HANDLER_H

#include "reference_handler.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initBranch();

void createUpdateRef();

void createBranch(char* branch);

int branchExists(char* branch);

void printBranch(char* branch);

List* branchList(char* branch);

char* getCurrentBranch();

List* branchList(char* branch) ;

List* getAllCommits();

void restoreCommit(char* hash_commit);

void myGitCheckoutBranch(char* branch);

void myGitCheckoutBranch(char* branch);

List* filterList(List* L, char* pattern);

void myGitCheckoutCommit(char* pattern);

char* hashToPathExtension(char* hash, char* extension);

#endif