#include "reference_handler.h"
#include "file_hash.h"
#include "list.h"
#include "branch_handler.h"
#include "merge_handler.h"
#include "utilities.h"

/**
 * La fonction main sert comme un menu où le client aura accès aux fonctions qui lui permettront de gérer 
 * l'historiel de versions de son projet. 
 */
int main(int argc, char** argv) {
    if (argc == 1) {
        fprintf(stderr,"Utilisation : %s <instruction> [complements]\n", argv[0]);
        fprintf(stderr,"%s help pour plus d'informations\n",argv[0]);
        return 0;
    } 

    char* programme = argv[0];
    char* instruction = argv[1];
    //affichage de tous les options
    if (equals(instruction, "help")) {
        print_color(stdout,"Options d'instructions :","blue");
        printf("%s init\n", programme);
        printf("%s list-refs\n", programme);
        printf("%s create-ref <name> <hash>\n", programme);
        printf("%s delete-ref <name>\n", programme);
        printf("%s add <elem> [<elem2> <elem3> ...]\n", programme);
        printf("%s list-add\n", programme);
        printf("%s clear-add\n", programme);
        printf("%s commit [-m \"<message>\"]\n", programme);
        printf("%s get-current-branch\n", programme);
        printf("%s branch <branch-name>\n", programme);
        printf("%s branch-print <branch-name>\n", programme);
        printf("%s checkout-branch <branch-name>\n", programme);
        printf("%s checkout-commit <pattern>\n", programme);
        printf("%s merge <branch> <message>\n", programme);
    } else if (equals(instruction, "init")) {
        initRefs();
        initBranch();
    } else if (equals(instruction, "list-refs")) {
        //./myGit list-refs
        List* l = listdir(".refs");
        if (l != NULL) {
            Cell* element = *l;
            printf(".refs :\n");
            while(element) {
                if (!equals(element->data,".") && !equals(element->data, "..")) {
                    printf("%s\n", element->data);
                }
                element = element->next;
            }
            freeList(l);
        } else {
            fprintf(stderr,"Directory .refs does not exist\n");
        }
    } else if (equals(instruction, "create-ref")) {
        //./myGit create-ref <name> <hash>
        if (argc != 4) {
            fprintf(stderr,"Utilisation : %s create-ref <name> <hash>\n", programme);
            exit(-1);
        }
        createUpdateRef(argv[2], argv[3]);
    } else if (equals(instruction, "delete-ref")) {
        //./myGit delete-ref <name>
        if (argc != 3) {
            fprintf(stderr, "Utilisation : %s delete-ref <name>", programme);
            exit(-1);
        }
        deleteRef(argv[2]);
    } else if (equals(instruction, "add")) {
        //./myGit add <files>
        if (argc < 3) {
            fprintf(stderr, "Utilisation : %s add <elem> [<elem2> <elem3> ...]", programme);
            exit(-1);
        }
        //on ajoute fichier par fichier
        for (int i = 2; i < argc; i++) {    
            char* file = argv[i];
            myGitAdd(file);
        }
    } else if (equals(instruction, "list-add")) { 
        if (access(".add", F_OK) == 0) {
            WorkTree* wt = ftwt(".add");
            //on recupere sous format worktree le contenu de .add
            char* str = wtts(wt);
            print_color(stdout,".add :","blue");
            //et on l'affiche
            printf("%s",str);
            free(str);
            freeWorkTree(wt);
        } else {
            print_color(stderr,"No .add yet", "red");
        }
    } else if (equals(instruction, "clear-add")) { 
        system("rm -f .add");
    } else if (equals(instruction, "commit")) { 
        if (argc < 3) {
            fprintf(stderr, "Utilisation : %s commit <branch_name> [-m <message>]\n", programme);
            exit(-1);
        }
        if (argc == 5 && equals(argv[3], "-m")) {
            myGitCommit(argv[2],argv[4]);
        } else if (argc == 3) {
            myGitCommit(argv[2], NULL);
        } else {
            fprintf(stderr,"Format error\nSyntax : %s commit <branch_name> [-m <message>]\n", programme);
        }
    } else if (equals(instruction, "get-current-branch")) {
        //./myGit get-current-branch
        char *str = getCurrentBranch();
        printf("Current branch : %s\n", str);
        free(str);
    } else if (equals(instruction, "branch")) {
        //./myGit branch <branch-name>
        if (argc != 3) {
            fprintf(stderr,"Utilisation : %s branch <branch-name>\n", programme);
            exit(-1);
        }
        char* branch = argv[2];
        if (branchExists(branch)) {
            fprintf(stderr,"Branch %s already exits\n", branch);
        } else {
            createBranch(branch);
        }
    } else if (equals(instruction, "branch-print")) {
        if (argc != 3) {
            fprintf(stderr,"Utilisation : %s branch-print <branch-name>\n", programme);
            exit(-1);
        }
        printBranch(argv[2]);
    } else if (equals(instruction, "checkout-branch")) {
        if (argc != 3) {
            fprintf(stderr,"Utilisation : %s checkout-branch <branch-name>\n", programme);
            exit(-1);
        }
        myGitCheckoutBranch(argv[2]);
    } else if (equals(instruction, "checkout-commit")) {
        if (argc != 3) {
            fprintf(stderr,"Utilisation : %s checkout-commit <pattern>\n", programme);
            exit(-1);
        }
        myGitCheckoutCommit(argv[2]);
    } else if (equals(instruction, "merge")) {
        //./myGit merge <branch> <message>
        if (argc < 3) {
            fprintf(stderr,"Utilisation : %s merge <branch> <message>\n", programme);
            exit(-1);
        }
        char* remote_branch = argv[2];
        char* current_branch = getCurrentBranch();
        char* msg = (argc >= 4 ? argv[3] : "");
        bool success = true;
        //peut etre que les deux branches coincident
        if (equals(remote_branch, current_branch)) {
            print_color(stderr,"Error, both branches refer to the current branch", "red");
            free(current_branch);
            exit(-1);
        }
        //on essaie de merger
        List* conflicts = merge(remote_branch, msg), *retry = NULL;
        if (*conflicts != NULL) {
            char opt;
            List* current_conflicts;
            List* remote_conflicts;
            //if there is a file overlap we have to choose how to solve the conflict (choose which branch to keep the files in)
            print_color(stderr, "! Merge error - one or multiple files are in conflict", "red");
            fprintf(stderr,"\033[0;33m");
            fprintf(stderr,"Select one of the following : \n"); 
            fprintf(stderr, "1. Save Files from Current Branch (a deletion commit will be done for %s) before merging\n", remote_branch);
            fprintf(stderr, "2. Save Files from Branch %s (a deletion commit will be done for the current branch) before merging\n", remote_branch);
            fprintf(stderr, "3. Choose files manually\n");
            fprintf(stderr, "\033[0m");
            scanf(" %c", &opt);
            switch(opt) {
                case '1':
                    createDeletionCommit(remote_branch, conflicts, msg);
                    printf("\033[0;32mDeletion commit created\n \033[0m"); 
                    retry = merge(remote_branch, msg);
                break;
                case '2':
                    createDeletionCommit(current_branch, conflicts, msg);
                    printf("\033[0;32mDeletion commit created\n \033[0m"); 
                    retry = merge(remote_branch, msg);
                break;
                case '3':
                    current_conflicts = initList();
                    remote_conflicts = initList();
                    char destination[300];
                    printf("Choose which file's version to keep\n");
                    for (Cell* e = *conflicts; e != NULL; e = e->next) {
                        char* file;
                        top: file = e->data;
                        //on choisi la branche associee a la version du fichier qu'on veut conserver
                        printf("\033[0;33mFile %s ? (%s | %s or q to quit) : \033[0m", file, current_branch, remote_branch);
                        scanf(" %s", destination);
                        //et on l'insere en tete de la liste
                        if (equals(current_branch, destination)) {
                            //si on choisi la version de la branche actuel alors c'est un conflit pour la branche remote
                            insertFirst(remote_conflicts, buildCell(file));
                        } else if (equals(remote_branch, destination)) {
                            //idem
                            insertFirst(current_conflicts, buildCell(file));
                        } else if (equals(destination, "q")) {
                            print_color(stderr,"Operation aborted", "red");
                            free(current_branch);
                            freeList(conflicts);
                            freeList(current_conflicts);
                            freeList(remote_conflicts);
                            exit(0);
                        } else {
                            //on ressaie
                            //nous utilisons goto uniquement pour éviter d'ajouter un niveau d'indentation inutile
                            goto top; 
                        }
                    }
                    printf("\033[0m");
                    createDeletionCommit(current_branch, current_conflicts, msg);
                    createDeletionCommit(remote_branch, remote_conflicts, msg);
                    printf("\033[0;32mTwo deletion commits created\n \033[0m"); 
                    retry = merge(remote_branch, msg);
                    freeList(current_conflicts);
                    freeList(remote_conflicts);
                break;
                default:
                    print_color(stderr,"Operation aborted", "red");
                    success = false;
                break;
            } 
            if (retry != NULL && *retry != NULL) {
                print_color(stderr, "Error while merging", "red");
                success = false;
            }
            freeList(retry);
        } else {
            print_color(stdout, "No conflicts found","green");
        }
        free(current_branch);
        freeList(conflicts);
        if (success)
            print_color(stdout, "Merge successful", "green");
    } else {
        fprintf(stderr, "command not found\n");
    }
    return 0;
}  