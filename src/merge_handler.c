#include "merge_handler.h"

/**
 * La fonction cree et renvoie un pointeur sur un worktree contenant les workfiles des fichiers de wt1
 * et wt2 qui ne sont pas en conflit. Les fichiers en conflit sont ajoutes a la liste conflicts.
 */
WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts) {
    *conflicts = initList();
    WorkTree* wt = initWorkTree();
    for (int i = 0; i < wt1->n; i++) {
        char* name = wt1->tab[i].name;
        int index = inWorkTree(wt2, name);
        if (index != -1 && strcmp(wt1->tab[i].hash, wt2->tab[index].hash)) {
            //si le Workfile est dans les deux commmits et les hashes sont differents
            insertFirst(*conflicts, buildCell(name));
        } else {
            //sinon on l'ajoute a la liste de fichiers sans conflit
            appendWorkTree(wt, name, wt1->tab[i].hash, wt1->tab[i].mode);
        }
    }
    //dans l'autre sens
    for (int i = 0; i < wt2->n; i++) {
        char* name = wt2->tab[i].name;
        if (inWorkTree(wt1, name) == -1) {
            //sinon on l'ajoute a la liste de fichiers sans conflit
            appendWorkTree(wt, name, wt2->tab[i].hash, wt2->tab[i].mode);
        }
    }
    return wt;
}

/**
 * Alloue un espace memoire pour reconstruire et 
 * renvoyer un pointeur du worktree du dernier 
 * commit de la branche branch
 */
WorkTree* getWorkTreeFromBranchName(char* branch) {
    /*
        a partir de la reference il faut extraire l'adresse du worktree du commit associe
    */
    WorkTree* wt;
    char* commit_hash = getRef(branch);
    if (commit_hash == NULL) {
        print_color(stderr,"Branch doesn't exist","red");
        exit(-1);
    }
    char* commit_path = hashToPathExtension(commit_hash, ".c");
    Commit* commit = ftc(commit_path);
    char* wt_hash = commitGet(commit,"tree");
    char* wt_path = hashToPathExtension(wt_hash, ".t");
    wt = ftwt(wt_path);
    free(wt_path);
    free(commit_hash);
    free(commit_path);
    freeCommit(commit);
    return wt;
}

/**
 *  Si il n'y a pas des conflits entre la remote branch et la branche actuelle, 
 * Un worktree wt contenant les fichiers des deux branches est alloue.
 * Un commit  ayant pour worktree  wt est crée et la current branch est actualisée,
 * elle pointe maintenant a ce nouveau commit. 
 * Ce commit aura pour predecesseur le dernier commit de la branche courante et 
 * additionellement il aura pour merged predecessor le dernier commit de remote_branch.
 *HEAD et current branch sont actualisés et wt est restaure.
 */
List* merge(char* remote_branch, char* message) {
    List* conflicts = NULL;
    
    char* current_branch = getCurrentBranch();
    WorkTree* current_wt = getWorkTreeFromBranchName(current_branch); 
    WorkTree* remote_wt = getWorkTreeFromBranchName(remote_branch);
    WorkTree* fusion = mergeWorkTrees(current_wt, remote_wt, &conflicts);

    if (*conflicts == NULL) {
        Commit* c = initCommit();
        //on recupere les predecesseurs
        char* merged_predecessor = getRef(remote_branch);
        char* predecessor = getRef(current_branch);

        commitSet(c, "message", message);
        commitSet(c, "predecessor", predecessor);
        commitSet(c, "merged_predecessor", merged_predecessor);
        
        char* hash_wt = blobWorkTree(fusion); 
        commitSet(c, "tree", hash_wt);

        char* hash_commit = blobCommit(c);
        createUpdateRef(current_branch, hash_commit);
        createUpdateRef("HEAD", hash_commit);

        deleteRef(remote_branch);

        restoreWorkTree(fusion, ".");

        freeCommit(c);
        free(hash_commit);
        free(hash_wt);
        free(merged_predecessor);
        free(predecessor);
    }
    freeWorkTree(current_wt);
    freeWorkTree(remote_wt);
    freeWorkTree(fusion);
    free(current_branch);
    return conflicts;
}

/**
 * La fonction va creer un commit sur la branche branch ou seulement les documents
 * absents dans la liste conflicts sont enregistres par le worktree du commit.
 */
void createDeletionCommit(char* branch, List* conflicts, char* message) {
    char* current_branch = getCurrentBranch();
    myGitCheckoutBranch(branch);
    char* hash_commit = getRef(branch);
    system("rm -f .add");
    WorkTree* wt = getWorkTreeFromBranchName(branch);
    for (int i = 0; i < wt->n; i++) {
        char* name = wt->tab[i].name;
        if (searchList(conflicts, name) == NULL) {
            myGitAdd(name);
        }
    }
    if (is_empty(".add")) {
        system("touch .padding");
        FILE* fp = fopen(".add","w");
        fprintf(fp, ".padding\t\t%d", getChmod(".padding"));
        fclose(fp);
    }
    myGitCommit(branch, message);
    myGitCheckoutBranch(current_branch);
    freeWorkTree(wt);
    free(hash_commit);
    free(current_branch);
}