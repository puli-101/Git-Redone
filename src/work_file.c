#include "work_file.h"

int getChmod ( const char * path ) {
    struct stat ret ;

    if ( stat ( path , & ret ) == -1) {
        return -1;
    }

    return ( ret.st_mode & S_IRUSR ) |( ret.st_mode & S_IWUSR ) |( ret.st_mode & S_IXUSR ) | /*owner*/
            ( ret.st_mode & S_IRGRP ) |( ret.st_mode & S_IWGRP ) |( ret.st_mode & S_IXGRP ) | /*group*/
            ( ret.st_mode & S_IROTH ) |( ret.st_mode & S_IWOTH ) |( ret.st_mode & S_IXOTH ) ; /*other*/
}

void setMode ( int mode , char * path ) {
    char buff [100];
    sprintf ( buff , "chmod %d %s " , mode , path ) ;
    system ( buff ) ;
}

WorkFile* createWorkFile(char* name) {
    WorkFile* wtf = (WorkFile*)malloc(sizeof(WorkFile));
    wtf->name = strdup(name);
    wtf->hash = sha256file(name);
    wtf->mode = getChmod(name);
    return wtf;
}

char* wfts(WorkFile* wf) {
    char* str = (char*)malloc(sizeof(char) * WF_STR_SIZE);
    char mode[100];

    str[0] = '\0';
    sprintf(mode,"%d",wf->mode);

    strcat(str,wf->name);
    strcat(str,"\t");
    strcat(str,wf->hash);
    strcat(str,"\t");
    strcat(str, mode);
    
    return str; 
}

//on alloue et renvoie un WF* a partir du char*
WorkFile* stwf(char* ch) {
    char name[WF_STR_SIZE];
    char hash[WF_STR_SIZE];
    char mode[WF_STR_SIZE];
    int i, j;
    for (i = 0; ch[i] != '\t' && ch[i] != '\0'; i++)
        name[i] = ch[i];
    name[i] = '\0';
    for (j = i + 1; ch[j] != '\t' && ch[j] != '\0'; j++)
        hash[j - i - 1] = ch[j];
    hash[j - i - 1] = '\0';
    for (i = j + 1; ch[i] != '\0'; i++)
        mode[i - j - 1] = ch[i];
    mode[i - j - 1] = '\0';

    WorkFile* wtf = (WorkFile*)malloc(sizeof(WorkFile));
    wtf->name = strdup(name);
    wtf->hash = strdup(hash);
    wtf->mode = atoi(mode);
    return wtf;
}

WorkTree* initWorkTree() {
    WorkTree* wt = (WorkTree*)malloc(sizeof(WorkTree));
    wt->tab = (WorkFile*)malloc(sizeof(WorkFile) * WF_TREE_SIZE);
    wt->size = WF_TREE_SIZE;
    wt->n = 0;
    return wt;
}

int inWorkTree(WorkTree* wt, char* name) {
    for (int i = 0; i < wt->n; i++) {
        if (!strcmp(wt->tab[i].name, name))
            return i;
    }
    return -1;
}

int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode) {
    int index = inWorkTree(wt,name);
    if (index != -1) 
        return index;
    if (wt->n >= wt->size)
        return -1;
    index = wt->n;
    wt->tab[index].name = strdup(name);
    wt->tab[index].hash = strdup(hash);
    wt->tab[index].mode = mode;
    (wt->n)++;
    return index;
}

char* wtts(WorkTree* wt) {
    char* str = (char*)malloc(sizeof(char) * WF_STR_SIZE * WF_TREE_SIZE);
    //int i = 0, k;
    for (int j = 0; j < wt->n; j++) {
        char* tmp = wfts(&(wt->tab[j]));
        /*for (k = i; k - i < strlen(tmp); k++) {
            str[k] = tmp[k - i];
        }
        if (i != 0) {
            str[k] = '\n';
        }
        i = k + 1;*/
        strcat(str,tmp);
        strcat(str,"\n");
        free(tmp);
    }
    //str[k] = '\0';
    return str;
}

void freeWorkFile(WorkFile* wf) {
    free(wf->name);
    free(wf->hash);
    free(wf);
}

//String to work tree
WorkTree* stwt(char* str) {
    char str_wf[WF_STR_SIZE];
    int index = 0;
    WorkTree* wt = initWorkTree();
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n' && index != 0) {
            str_wf[i] = '\0';
            WorkFile* wf = stwf(str_wf);
            printf("!str_wf : %s\n", str_wf);
            printf("!wfts : %s\n", wfts(wf));
            appendWorkTree(wt,wf->name, wf->hash, wf->mode);
            freeWorkFile(wf);
            index = 0;
        } else if (str[i] != '\n') {
            str_wf[index] = str[i];
            index++;
        }
    }
    return wt;
}

int wttf(WorkTree* wt, char* file) {
    FILE* f = fopen(file, "w");
    assert(f != NULL);
    char* str = wtts(wt);
    fprintf(f,"%s", str);
    free(str);
    fclose(f);
    return 1;
}