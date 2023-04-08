#ifndef MERGE_HANDLER_H
#define MERGE_HANDLER_H

#include "branch_handler.h"
#include "list.h"

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts);

List* merge(char* remote_branch, char* message);

void createDeletionCommit(char* branch, List* conflicts, char* message);

#endif