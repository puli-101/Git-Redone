#include "merge_handler.h"

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts) {
    *conflicts = initList();
    WorkTree* wt = initWorkTree();
    for (int i = 0; i < wt1->n; i++) {
        char* name = wt1->tab[i].name;
        if (inWorkTree(wt2, name) != -1) {
            //si le Workfile est dans les deux commmits
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

WorkTree* getWokTreeFromBranchName(char* branch) {
    /*
        a partir de la reference il faut extraire l'adresse du worktree du commit associe
    */
    WorkTree* wt;
    char* commit_hash = getRef(branch);
    char* commit_path = hashToPathExtension(commit_hash, ".c");
    Commit* commit = ftc(commit_path);
    char* wt_hash = commitGet(commit,"tree");
    char* wt_path = hashToPathExtension(wt_hash, ".t");
    wt = ftwt(wt_path);
    free(wt_path);
    free(commit_hash);
    free(commit_handler);
    freeCommit(remote_commit);
    return wt;
}

List* merge(char* remote_branch, char* message) {
    Liste* conflicts;
    
    char* current_branch = getCurrentBranch();
    WorkTree* current_wt = getWokTreeFromBranchName(current_branch); 
    WorkTree* remote_wt = getWokTreeFromBranchName(remote_branch);
    WorkTree* fusion = mergeWorkTrees(current_wt, remote_wt, &conflicts);

    if (*conflicts == NULL) {
        Commit* c = initCommit();
        //on recupere les predecesseurs
        char* merged_predecessor = getRef(remote_branch);
        char* predecessor = getRef(current_branch);

        setCommit(c, "message", message);
        setCommit(c, "predecessor", predecessor);
        setCommit(c, "merged_predecessor", merged_predecessor);
        
        char* hash_wt = blobWorkTree(fusion); 
        setCommit(c, "tree", hash_wt);

        char* hash_commit = blobCommit(c);
        createUpdateRef(current_branch, hash_commit);
        createUpdateRef("HEAD", hash_commit);

        deleteRef(remote_branch);

        restoreWorkTree(fusion);

        free(hash_commit);
        free(hash_wt);
        free(merged_predecessor);
        free(predecessor);
    }
    freeWorkTree(fusion);
    free(current_branch);
    return current_branch;
}