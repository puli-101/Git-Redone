#include "branch_handler.h"

/**
 * Duplique le master dans le fichier caché current_branch.
 */
void initBranch() {
    system("echo master > .current_branch");
}

/**
 * Vérifie si la branche branch existe dans le répertoire de références. 
 */
int branchExists(char* branch) {
    char buff[300] = ".refs/";
    strcat(buff,branch);
    return access(buff, F_OK) == 0;
}

/**
 * Fait que le head pointe dans la branche indiquée. 
 */
void createBranch(char* branch) {
    char* ref = getRef("HEAD");
    createUpdateRef(branch, ref);
    free(ref);
}

/**
 * Alloue un espace de mémoire pour garder et renvoyer un pointeur sur le hash du dernier commit qui 
 * est stocké aussi dans le master en théorie. 
 */
char* getCurrentBranch() {
    FILE* f = fopen(".current_branch", "r");
    assert(f != NULL);
    char* current = (char*)malloc(sizeof(char)*200);
    fgets(current, 200, f);
    fclose(f);
    return current;
}

//utilities.h
/**
 *Alloue et renvoie la direction d'un fichier à partir de son hash et ajoute l'extension indiquée à 
 *la fin de l'adresse.
 */
char* hashToPathExtension(char* hash, char* extension) {
    char* str = hashToPath(hash);
    strcat(str,extension);
    return str;
}

/**
 *  Pour une branche, la fonction va afficher le message et le hash de tous les commits de la branche.
 * L'affichage est effectué en allant du plus nouveau des commits au plus vieux des commits.
 * des commits de la branche. Les informations sont présentées sous la forme:
 * Hash: ...
 * Message: ...
 */
void printBranch(char* branch) {
    char* commit_hash = getRef(branch);
    if (!commit_hash) return;
    char* path = hashToPathExtension(commit_hash, ".c");
    printf("! %s \n", path);
    Commit *c = ftc(path);
    while(c) {
        char* message = commitGet(c,"message");
        
        if (message != NULL) {
            printf("Hash : %s\nMessage : %s\n", commit_hash, message);
        } else {
            printf("Hash : %s\nMessage : [empty]\n", commit_hash);
        }
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

/**
 * Initialise et alloue l'espace de mémoire pour rendre le pointeur sur une liste contenant
 * dans ces cellules les hash des commits de la branche branch.
 * Les commits dans la liste vont être ordonnées dans leur ordre chronologique (Le plus vieux dans
 * la tête de la liste.)
 */
List * branchList(char* branch) {
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

/**
 * Cette fonction renvoie un pointeur sur une liste avec tous les commits de toutes les branches
 * de ./refs .La fonction va parcourir les listes de commits de chaque branche et ajouter les commits dans 
 * une liste L si ils ne sont pas déjà dans la liste.
 * La fonction renvoie le pointeur de L.
 */
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

/**
 * La fonction va chercher le worktree du commit de hash commit et le restaurer.
 */
void restoreCommit(char* hash_commit) {
    char *commit_path = hashToPathExtension(hash_commit, ".c") ;
    Commit *c = ftc (commit_path);

    char* wt_hash = strdup(commitGet (c , "tree"));
    char *wt_path = hashToPathExtension(wt_hash, ".t");

    WorkTree * wt = ftwt (wt_path);

    restoreWorkTree(wt , ".");

    freeCommit(c);
    freeWorkTree(wt);
    free(commit_path);
    free(wt_hash);
    free(wt_path);
}

/**
 * Va écrire le nom de la branche mise en paramètre dans le fichier ".current_branch"
 * et elle actuelise HEAD pour qu'elle point sur la nouvelle branche courrante.
 * Un message d'erreur est affiché si la branche n'existe pas dans ./refs.
 */
void myGitCheckoutBranch(char* branch) {
    FILE* f = fopen(".current_branch", "w");
    fprintf(f, "%s", branch);
    fclose(f);

    char* commit_hash = getRef(branch);
    if (commit_hash == NULL || commit_hash[0] == '\0') {
        fprintf(stderr,"Invalid branch %s\n",branch);
        return;
    }
    createUpdateRef("HEAD", commit_hash);

    restoreCommit(commit_hash);
    free(commit_hash);
}

/**
 * La fonction renvoie 1 si le paramètre prefix est un prefixe du paramètre word.
 * Au cas contraire un 0 est renvoyé.
 */
int isPrefix(char* prefix, char* word) {
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (word[i] == '\0' || prefix[i] != word[i])
            return 0;
    }
    return 1;
}

/**
 * Renvoie un pointeur sur une sous liste l de L où toutes les cellules de l ont pour
 * préfixe les charactères du paramètre pattern. La fonction fait une copie des éléments de 
 * L.
 */
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

/**
 * Selon le patron inséré en paramètres, la fonction va afficher la liste des commits qui suivent
 * ont pour préfixe pattern dans leur hash. S'il ne reste qu'un commit ayant le patron pour préfixe, la
 * Head va pointer sur ce commit et ce Commit est restauré.
 * Si aucun commit a pour préfixe pattern un message d'erreur est affiché.
 */
void myGitCheckoutCommit(char* pattern) {
    List* all_commits = getAllCommits();
    List* filter = filterList(all_commits, pattern);
    freeList(all_commits);

    if (*filter == NULL) {
        fprintf(stderr,"Error : commit not found\n");
        freeList(filter);
        return;
    } else if ((*filter)->next == NULL) {
        //s'il ne reste qu'un commit dans la liste
        char* commit_hash = (*filter)->data;
        createUpdateRef("HEAD", commit_hash);
        restoreCommit(commit_hash);
    } else {
        printf("Possible matches for pattern %s :\n", pattern);
        for (Cell* e = *filter; e != NULL; e = e->next)
            printf("%s\n", e->data);
    }

    freeList(filter);
}