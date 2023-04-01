#include "reference_handler.h"

void initRefs() {
    system("mkdir -p .refs");
    system("touch .refs/master");
    system("touch .refs/HEAD");
}

void createUpdateRef(char* ref_name, char* hash) {
    char buff[300] = ".refs/";
    strcat(buff,ref_name);
    FILE* f = fopen(buff, "w");
    fprintf(f, "%s", hash);
    fclose(f);
}

void deleteRef(char* ref_name) {
    char cmd[500];
    sprintf(cmd,"rm -f .refs/%s", ref_name);
    system(cmd);
}

char* getRef(char* ref_name) {
    char buff[300] = ".refs/";
    strcat(buff,ref_name);
    if (access(buff, F_OK) == 0) {
        char* content = (char*)malloc(sizeof(char)*500);
        FILE* f = fopen(buff, "r");

        if (fgets(content,500,f) == NULL) {
            content[0] = '\0';
        }
        
        fclose(f);
        return content;
    } else {
        fprintf(stderr,"Reference .refs/%s does not exist\n", ref_name);
        return NULL;
    }
}

void myGitAdd(char* file_or_folder) {
    system("touch .add");
    if (access(file_or_folder, F_OK) == 0) {
        WorkTree* add = ftwt(".add");
        appendWorkTree(add, file_or_folder, "", getChmod(file_or_folder));
        wttf(add, ".add");
        freeWorkTree(add);
    } else {
        fprintf(stderr,"Error : file %s does not exist\n", file_or_folder);
    }
}

void myGitCommit(char* branch_name, char* message) {
    if (access(".refs", F_OK) != 0) {
        fprintf(stderr,"“Initialiser d'abord les references du projet\n");
        exit(-1);
    } 
    char buff[300] = ".refs/";
    strcat(buff,branch_name);
    if (access(buff, F_OK) != 0) {
        fprintf(stderr, "La branche n'existe pas\n");
        exit(-1);
    } 

    char* last_commit = getRef(branch_name);
    char* head_commit = getRef("HEAD");
    if (strcmp(head_commit,last_commit) != 0) { //s'ils ne pointent pas vers la meme chose
        fprintf(stderr, "“HEAD doit pointer sur le dernier commit de la branche\n");
        exit(-1);
    }
    
    WorkTree* wt = ftwt(".add");
    char* hashWT;
    system("rm .add");
    hashWT = saveWorkTree(wt, ".");
    Commit *c = createCommit(hashWT);

    if (strlen(last_commit) != 0) {
        commitSet(c,"predecessor", last_commit);
    }
    if (message != NULL) { //commit[0] != '\0' ?
        commitSet(c, "message", message);
    }
    char* hashCommit = blobCommit(c);

    createUpdateRef(branch_name, hashCommit);
    createUpdateRef("HEAD", hashCommit);

    free(hashCommit);
    free(hashWT);
    free(last_commit);
    free(head_commit);
}