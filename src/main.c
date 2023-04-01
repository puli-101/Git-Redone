#include "reference_handler.h"
#include "file_hash.h"
#include "list.h"
#include "branch_handler.h"

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
            fprintf(stderr, "Utilisation : %s commit <branch_name> [-m <message>]", programme);
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
    }
    return 0;
}