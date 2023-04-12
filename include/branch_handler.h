#ifndef BRANCH_HANDLER_H
#define BRANCH_HANDLER_H

#include "reference_handler.h"
#include "list.h"
#include "utilities.h"

void initBranch();

void createUpdateRef();

void createBranch(char* branch);

int branchExists(char* branch);

void printBranch(char* branch);

List* branchList(char* branch);

char* getCurrentBranch();

List* branchList(char* branch) ;

List* getAllCommits();

void myGitCheckoutBranch(char* branch);

void myGitCheckoutBranch(char* branch);

void myGitCheckoutCommit(char* pattern);

#endif