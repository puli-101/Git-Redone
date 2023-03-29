#include "branch_handler.h"

void initBranch() {
    system("echo master > .current_branch");
}

int branchExists(char* branch) {
    char buff[300] = ".refs/";
    strcat(buff,branch);
    return access(buff, F_OK) == 0;
}