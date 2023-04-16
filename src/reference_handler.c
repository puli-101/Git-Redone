#include "reference_handler.h"

/** Cette fonction va creer le repertoire caché de reférences si il n'existe pas.
 * Dans ce repertoire, les fichiers vides  Head et Master sont crees. */
void initRefs() {
    system("mkdir -p .refs");
    system("touch .refs/master");
    system("touch .refs/HEAD");
}

/** Le fichier de nom ref_name est ouvert ou cree s'il n'existe pas dans le repertoire de 
 *références. hash est écrit dessus ce fichier. */
void createUpdateRef(char* ref_name, char* hash) {
    char buff[300] = ".refs/";
    strcat(buff,ref_name);
    FILE* f = fopen(buff, "w");
    if (f == NULL) {
        print_color(stderr,".refs not initialized !", "red");
        exit(-1);
    }
    fprintf(f, "%s", hash);
    fclose(f);
}

/** Elimine la reference mise en parametres. */
void deleteRef(char* ref_name) {
    char cmd[500];
    sprintf(cmd,"rm -f .refs/%s", ref_name);
    system(cmd);
}

/** Si ref_name existe dans le fichier ./refs, un pointeur su une chaine de caracteres allouee
 * contenant le texte du fichier ref_name est renvoye. */
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

/** Ajoute le fichier file_or_folder au WorkTree decrit par ".add", notre zone de preparation. */
void myGitAdd(char* file_or_folder) {
    system("touch .add");
    if (access(file_or_folder, F_OK) == 0) { //vérification de l'existance de add
        WorkTree* add = ftwt(".add");        //recuperation du worktree represente par add
        appendWorkTree(add, file_or_folder, "", getChmod(file_or_folder)); //ajout de file_or_folder
        wttf(add, ".add");  //enregistrement du nouveau worktree dans add
        freeWorkTree(add);
    } else {
        fprintf(stderr,"Error : file %s does not exist\n", file_or_folder); 
    }
}

/** Cree le commit pour message @message et pour worktree le worktree de ".add" sur la 
 * branche branch_name. */
void myGitCommit(char* branch_name, char* message) {
    if (access(".refs", F_OK) != 0) {              //vérification de l'existance des references
        fprintf(stderr,"Initialiser d'abord les references du projet\n");
        exit(-1);
    } 
    char buff[300] = ".refs/";
    strcat(buff,branch_name);
    if (access(buff, F_OK) != 0) {      //verification de l'acces a branch_name
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
    
    if (access(".add", F_OK) != 0) {    //acces au worktree qu'on devrait enregistrer
        fprintf(stderr, "No files to be committed\n");
        free(last_commit);
        free(head_commit);
        exit(-1);
    }
    WorkTree* wt = ftwt(".add");
    char* hashWT;
    system("rm .add");
    hashWT = saveWorkTree(wt, ".");     //sauvegarde d'une copie des documents de .add
    Commit *c = createCommit(hashWT);   //creation du commit 

    if (strlen(last_commit) != 0) {
        commitSet(c,"predecessor", last_commit);
    }
    if (message != NULL) { //commit[0] != '\0' ?
        commitSet(c, "message", message);
    }
    char* hashCommit = blobCommit(c);

    createUpdateRef(branch_name, hashCommit);   //actualisation de nos branches
    createUpdateRef("HEAD", hashCommit);

    freeCommit(c);
    freeWorkTree(wt);
    free(hashCommit);
    free(hashWT);
    free(last_commit);
    free(head_commit);
}