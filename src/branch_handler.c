#include "branch_handler.h"

void initBranch() {
    system("echo master > .current_branch");
}

int branchExists(char* branch) {
    char buff[300] = ".refs/";
    strcat(buff,branch);
    return access(buff, F_OK) == 0;
}

void createBranch(char* branch) {
    char* ref = getRef("HEAD");
    createUpdateRef(branch, ref);
    free(ref);
}

char* getCurrentBranch() {
    FILE* f = fopen(".current_branch", "r");
    assert(f != NULL);
    char* current = (char*)malloc(sizeof(char)*200);
    fgets(current, 200, f);
    fclose(f);
    return current;
}

//utilities.h
char* hashToPathExtension(char* hash, char* extension) {
    char* str = hashToPath(hash);
    strcat(hash,extension);
    return str;
}

void printBranch(char* branch) {
    char* commit_hash = getRef(branch);
    if (!commit_hash) return;
    char* path = hashToPathExtension(commit_hash, ".c");
    Commit *c = ftc(path);
    while(c) {
        char* message = commitGet(c,"message");
        if (!message) {
            message = (char*)malloc(sizeof(char));
            message[0] = '\0';
        }
        printf("Hash : %s\nMessage : %s\n", commit_hash, message);

        free(message);
        free(path);
        free(commit_hash);
        
        char* tmp = commitGet(c, "predecessor");
        if (tmp) {
            commit_hash = strdup(tmp);
            freeCommit(c);

            path = hashToPathExtension(commit_hash,".c");
            c = ftc(path);
        } else {
            freeCommit(c);
            c = NULL;
        }
    }
    if (c != NULL) {
        freeCommit(c);
        free(path);
        free(commit_hash);
    }
}

List* branchList(char* branch) {
    List* l = initList();
    char* commit_hash = getRef(branch);
    char* path = hashToPathExtension(commit_hash, ".c");
    Commit *c = ftc(path);
    while(c) {
        insertFirst(l, buildCell(commit_hash));
        free(path);
        free(commit_hash);
        
        char* tmp = commitGet(c, "predecessor");
        if (tmp) {
            commit_hash = strdup(tmp);
            freeCommit(c);

            path = hashToPathExtension(commit_hash,".c");
            c = ftc(path);
        } else {
            freeCommit(c);
            c = NULL;
        }
    }
    if (c != NULL) {
        freeCommit(c);
        free(path);
        free(commit_hash);
    }
    return l;
}

List* getAllCommits() {
    List* l = initList();
    List* refs = listdir(".refs");
    for (Cell* c = *refs; c != NULL; c = c->next) {
        char* ref  = c->data;
        if (*ref == '.') continue;

        List* branch = branchList(ref);
        for (Cell* e = *branch; e != NULL; e = e->next) {
            if (!searchList(l, e->data)) {
                insertFirst(l, buildCell(e->data));
            }
        }
        freeList(branch);
    }
    freeList(refs);
    return l;
}

void myGitCheckoutBranch(char* branch) {

}

void restoreCommit(char* hash_commit) {
    char *commit_path = hashToPathExtension(hash_commit, ".c") ;
    Commit *c = ftc (commit_path);

    char* wt_hash = strdup(commitGet (c , "tree"));
    char *wt_path = hashToPathExtension(wt_hash, ".t");

    WorkTree * wt = ftwt (wt_path) ;
    restoreWorkTree(wt , ".");

    freeWorkTree(wt);
    free(commit_path);
    free(wt_hash);
    free(wt_path);
}

void myGitCheckoutBranch(char* branch) {
    FILE* f = fopen(".current_branch");
    fprintf(f, "%s", branch);
    fclose(f);

    char* commit_hash = getRef(branch);
    createUpdateRef("HEAD", commit_hash);
    free(commit_hash);

    restoreCommit(commit_hash);
}

int isPrefix(char* prefix, char* word) {
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (word[i] == '\0' || prefix[i] != word[i])
            return 0;
    }
    return 1;
}

List* filterList(List* L, char* pattern) {
    List* l = initList();
    for (Cell* e = *L; e != NULL; e = e->next) {
        char* str = e->data;
        if (isPrefix(pattern, str)) {
            insertFirst(l, buildCell(str));
        }
    }
    return l;
}

void myGitCheckoutCommit(char* pattern) {
    List* all_commits = getAllCommits();
    List* filter = filterList(all_commits, pattern);
    freeList(all_commits);

    if (*filter == NULL) {
        fprintf(stderr,"Error : commit not found\n");
        return;
    } else if ((*filter)->next == NULL) {
        //s'il ne reste qu'un commit dans la liste
        
    } else {

    }

    free(all_commits);
}