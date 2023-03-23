#include "reference_handler.h"

void initRefs() {
    system("mkdir -p .refs");
    system("touch .refs/master");
    system("touch .refs/HEAD");
}

void createUpdateRef(char* ref_name, char* hash) {
    FILE* f = fopen(ref_name, "w");
    fprintf(f, "%s", hash);
    fclose(f);
}

void deleteRef(char* ref_name) {
    char cmd[500];
    sprintf(cmd,"rm -f %s", ref_name);
    system(cmd);
}

char* getRef(char* ref_name) {
    if (access(ref_name, F_OK) == 0) {
        char* content = (char*)malloc(sizeof(char)*500);
        FILE* f = fopen(ref_name, "r");

        if (fgets(content,500,f) == NULL) {
            content[0] = '\0';
        }
        
        fclose(f);
        return content;
    } else {
        return NULL;
    }
}