#ifndef REFERENCE_HANDLER_H
#define REFERENCE_HANDLER_H

#include "utilities.h"
#include "commit_handler.h"
#include "worktree_handler.h"

void initRefs();

void createUpdateRef(char* ref_name, char* hash);

void deleteRef(char* ref_name);

char* getRef(char* ref_name);

void myGitAdd(char* file_or_folder);

void myGitCommit(char* branch_name, char* message);

#endif