#include "reference_handler.h"
#include "file_hash.h"
#include "list.h"
#include "branch_handler.h"
#include "merge_handler.h"

int equals(char* str1, char* str2) {
    return !strcmp(str1, str2);
}

int main(int argc, char** argv) {
    if (argc == 1) {
        fprintf(stderr,"Utilisation : %s <instruction> [complements]\n", argv[0]);
        fprintf(stderr,"%s help pour plus d'informations\n",argv[0]);
        return 0;
    } 

    char* programme = argv[0];
    char* instruction = argv[1];
    if (equals(instruction, "help")) {
        printf("Options d'instructions : \n");
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
        if (argc != 4) {
            fprintf(stderr,"Utilisation : %s create-ref <name> <hash>\n", programme);
            exit(-1);
        }
        createUpdateRef(argv[2], argv[3]);
    } else if (equals(instruction, "delete-ref")) {
        if (argc != 3) {
            fprintf(stderr, "Utilisation : %s delete-ref <name>", programme);
            exit(-1);
        }
        deleteRef(argv[2]);
    } else if (equals(instruction, "add")) {
        if (argc < 3) {
            fprintf(stderr, "Utilisation : %s add <elem> [<elem2> <elem3> ...]", programme);
            exit(-1);
        }
        
        for (int i = 2; i < argc; i++) {
            char* file = argv[i];
            myGitAdd(file);
        }
    } else if (equals(instruction, "list-add")) { 
        WorkTree* wt = ftwt(".add");
        char* str = wtts(wt);
        printf(".add : \n%s\n",str);
        free(str);
        freeWorkTree(wt);
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
            fprintf(stderr,"Format error\n");
        }
    } else if (equals(instruction, "get-current-branch")) {
        //./myGit get-current-branch
        char *str = getCurrentBranch();
        printf("Current branch : %s", str);
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
        char* msg = (argc >= 4 ? argv[3] : "");
        List* conflicts = merge(remote_branch, msg), *retry = NULL;
        if (conflicts != NULL) {
            int opt;
            char* current_branch = getCurrentBranch();
            fprintf(stderr,"\033[0;31m"); //Set the text to the color red
            fprintf(stderr,"Merge error\n"); 
            fprintf(stderr,"\033[0;33m");
            fprintf(stderr,"Select one of the following : \n"); 
            fprintf(stderr, "1. Save Files from Current Branch (a deletion commit will be done for %s) before merging\n", remote_branch);
            fprintf(stderr, "2. Save Files from Branch %s (a deletion commit will be done for the current branch) before merging\n", remote_branch);
            fprintf(stderr, "3. Save Files from Current Branch\n");
            fprintf(stderr, "\033[0m");
            scanf(" %d", &opt);
            switch(opt) {
                case 1:
                    createDeletionCommit(remote_branch, conflicts, msg);
                    retry = merge(remote_branch, msg);
                break;
                case 2:
                    createDeletionCommit(current_branch, conflicts, msg);
                    retry = merge(remote_branch, msg);
                break;
                case 3:
                    fprintf(stderr,"npi\n");
                break;
                default:
                    fprintf(stderr,"Operation aborted\n");
                break;
            }
            if (retry != NULL) {
                freeList(retry);
                fprintf(stderr,"Error while merging\n");
            }
            free(current_branch);
            freeList(conflicts);
        }
    } else {
        fprintf(stderr, "command not found\n");
    }
    return 0;
}