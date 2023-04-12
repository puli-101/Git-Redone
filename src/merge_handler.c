#include "merge_handler.h"

/**
 * La fonction renvoie unpointeur sur un worktree alloué qui contient dans sa table
 * les workfiles qui ne sont pas présentes dans wt1 et wt2. À la fin de la fonction 
 * conflicts era un double pointeur sur une liste contennant les noms des workfiles 
 * présents dans le wt1 et wt2.
 * On considère qu'un workfile wf est dans wt1 et wt2 si les deux worktrees ont un 
 * workfile avec le hash de wf.
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
 * Alloue un espace mémoire pour recontruire et 
 * renvoyer un pointeur du worktree du dernier 
 * commit de la branche branch
 */
WorkTree* getWorkTreeFromBranchName(char* branch) {
    /*
        a partir de la reference il faut extraire l'adresse du worktree du commit associe
    */
    WorkTree* wt;
    char* commit_hash = getRef(branch); //si branch absente dans ref?
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
 *  Si il n'y a pas des conflits entre la remote branch et la nouvelle branche, 
 * Un worktree wt contenant les fichiers des deux branches est alloué.
 * Un commit  ayant pour worktree  wt est crée et la current branch est actualisée,
 * elle pointe maintenant à ce nouveau commit. 
 * Ce commit aura pour prédecesseur le dernier commit de la branche courante et 
 * additionellement il aura pour merged predecessor le dernier commit de remote_branch.
 * Le HEAD va aussi pointer ver le nouveau commit. Finalement l'arbre wt est restauré.
 * La fonction renvoie un pointeur sur la liste de conflits entre remote_branch et la 
 * branche courrante. 
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

        free(hash_commit);
        free(hash_wt);
        free(merged_predecessor);
        free(predecessor);
    }
    freeWorkTree(fusion);
    free(current_branch);
    return conflicts;
}

/**
 * La fonction va créer un commit sur la branche branch où seulement les documents
 * absents dans la liste de  conflits sont enregistrés par le worktree du commit.
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
    myGitCommit(branch, message);
    myGitCheckoutBranch(current_branch);
    freeWorkTree(wt);
    free(hash_commit);
    free(current_branch);
}