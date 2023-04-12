#include "reference_handler.h"
/**
 * Cette fonction va créer le repertoire caché de reférences si il n'existe pas.
 * Dans ce repertoire, les fichiers vides  Head et Master sont crées.
 */
void initRefs() {
    system("mkdir -p .refs");
    system("touch .refs/master");
    system("touch .refs/HEAD");
}

/**
 *Le fichier de nom ref_name est ouvert ou crée s'il n'existe pas dans le repertoire de 
 *références. hash est écrit dessus ce fichier. Maintenant il est une référence correspondant 
 *à un fichier ayant ce hash. 
 */
void createUpdateRef(char* ref_name, char* hash) {
    char buff[300] = ".refs/";
    strcat(buff,ref_name);
    FILE* f = fopen(buff, "w");
    fprintf(f, "%s", hash);
    fclose(f);
}

/**
 * Élimine la reférence mise en paramètres.
 */
void deleteRef(char* ref_name) {
    char cmd[500];
    sprintf(cmd,"rm -f .refs/%s", ref_name);
    system(cmd);
}

/**
 * Pour une référence existante dans ./refs, la référence ref_name est ouverte.
 * Son contenu est lu et alloué dans une chaine de caractères de taille maximale 500.
 * La fonction renvoie cette chaine de caractères si le fichier existe. Si non un message
 * d'erreur est affiché.
 */
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

/**
 * Va créer o un worktree correspondant à notre zone de préparation, c'est à dire
 * le repertoire chaché ./add .
 * Le fichier en paramètres sera ajouté à ce worktree si possible.
 * La zone de préparation sera actualisé et le document file_ro_folder sera inclus dans
 * la zone.
 * Si file_or_folder n'existe pas un message d'erreur est affiché.
 */
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

/**
 *Pour que la fonction fonctionne il faut que Head et brach name existent et aient le même contenu.
 *Les répertoires ./add et ./refs doivent exister. Si non un message d'erreur est affiché.
 * La fonciton va créer un worktree de ./add et le sauvegarder. 
 * La zone de préparation sera eliminé et un commit représentant (par so hash) le worktree sauvegardé est 
 * crée. Ce commit est suvegardé et ajouté au début de  la branche branch_name. Le head est placé sur le 
 * nouveau commit.
 */
void myGitCommit(char* branch_name, char* message) {
    if (access(".refs", F_OK) != 0) {
        fprintf(stderr,"Initialiser d'abord les references du projet\n");
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
        fprintf(stderr, "HEAD doit pointer sur le dernier commit de la branche\n");
        free(last_commit);
        free(head_commit);
        exit(-1);
    }
    
    if (access(".add", F_OK) != 0) {
        fprintf(stderr, "No files to be committed\n");
        free(last_commit);
        free(head_commit);
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

    freeCommit(c);
    freeWorkTree(wt);
    free(hashCommit);
    free(hashWT);
    free(last_commit);
    free(head_commit);
}