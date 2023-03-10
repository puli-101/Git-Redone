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
    char buff [1000];
    sprintf ( buff , "chmod %o %s " , mode , path ) ;
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
//on suppose que char* est dans le bon format
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
    str[0] = '\0';

    for (int j = 0; j < wt->n; j++) {
        char* tmp = wfts(&(wt->tab[j]));
        strcat(str,tmp);
        strcat(str,"\n");
        free(tmp);
    }
    strcat(str,"\n");
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
    int index = 0, i;
    WorkTree* wt = initWorkTree();
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n' && index != 0) {
            str_wf[index] = '\0';
            WorkFile* wf = stwf(str_wf);
            appendWorkTree(wt,wf->name, wf->hash, wf->mode);
            freeWorkFile(wf);
            index = 0;
        } else if (str[i] != '\n') {
            str_wf[index] = str[i];
            index++;
        }
    }
    if (index != 0) {
        str_wf[index] = '\0';
        WorkFile* wf = stwf(str_wf);
        appendWorkTree(wt,wf->name, wf->hash, wf->mode);
        freeWorkFile(wf);
    }
    return wt;
}

//WorkTree* to File
int wttf(WorkTree* wt, char* file) {
    FILE* f = fopen(file, "w");
    assert(f != NULL);
    char* str = wtts(wt);
    fprintf(f,"%s", str);
    free(str);
    fclose(f);
    return 1;
}

//File to WorkTree*
//on suppose que chaque ligne a le bon format
WorkTree* ftwt(char* file) {
    FILE* f = fopen(file, "r");
    char buff[WF_STR_SIZE];
    WorkTree* wt = initWorkTree();

    while (fgets(buff, WF_STR_SIZE, f)) {
        //on ignore les lignes vides
        if (!strcmp(buff,"\n"))
            continue;
        WorkFile* wf = stwf(buff);
        if (wf != NULL)
            appendWorkTree(wt,wf->name, wf->hash, wf->mode);
        freeWorkFile(wf);
    }
    fclose(f);
    return wt;
}

//libere work tree
void freeWorkTree(WorkTree* wt) {
    for (int i = 0; i < wt->n; i++) {
        free(wt->tab[i].name);
        free(wt->tab[i].hash);
    }
    free(wt->tab);
    free(wt);
}

//retourner le hash du fichier temporaire qui contient le
char* blobWorkTree(WorkTree* wt) {
    static char template [] = "tmpXXXXXX" ;
	char temp_wt_file [HASH_STR_SIZE - 100];
    char rep[3];
    char* instantanee;
	char cmd [HASH_STR_SIZE];
	char *sha = (char*)malloc(HASH_STR_SIZE*sizeof(char));
    char* hash;

	if (sha == NULL) 
		return NULL;
	
	strcpy (temp_wt_file , template) ;
	
	//On cree le fichier tmpX...
	int fd = mkstemp (temp_wt_file) ;

	if (fd == -1) {
		free(sha);
        close(fd);
		return NULL;
	}

    close(fd);

    //On stocke la version string du WorkTree dans le temp_wt_file
    wttf(wt, temp_wt_file);

    //On recupere le hash du workfile stocke dans temp_wt_file
    sha = sha256file(temp_wt_file);

    //On recupere l'adresse correspondant au fichier instantanee
    hash = sha256file(temp_wt_file);
    instantanee = hashToPath(hash);
    free(hash);
    
    strcat(instantanee, ".t");
    //On recupere le nom du repertoire
    rep[0] = instantanee[0];
    rep[1] = instantanee[1];
    rep[2] = '\0';
    //On cree le repertoire
    sprintf(cmd, "mkdir -p %s", rep);
    system(cmd);
    //On copie le fichier 'file' vers 'instantanee'
    cp(instantanee, temp_wt_file);
    free(instantanee);

    //On supprime le fichier temporaire
    sprintf(cmd,"rm %s",temp_wt_file);
	system(cmd);

	return sha;
}

int is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

char* saveWorkTree(WorkTree* wt, char* path) {
    char file_path[WF_STR_SIZE * 2];
    char* file;

    printf("SaveWorkTree at %s\n",path);
    for (int i = 0; i < wt->n; i++) {
        file = wt->tab[i].name;

        if (is_regular_file(file)) {
            printf("\tSaving file %s\n", file);
            //on stocke l'instantanee du fichier
            blobFile(file);
            //si jamais on avait deja une chaine correspondant au hash enregistre dans le tas on libere la memoire
            free(wt->tab[i].hash);
            //on recupere le hash du fichier 
            wt->tab[i].hash = sha256file(file);

        } else {    //on suppose qu'il n'y a pas de tubes ni de links
            printf("\tSaving directory %s\n", file);
            WorkTree* new_wt = initWorkTree();
            List* l = listdir(file);
            Cell* element = *l;
            //on liste tous les elements du repertoire (en ignorant . et ..)
            while (element) {
                if (!strcmp(element->data,"..") || !strcmp(element->data,".")) {
                    element = element->next;
                    continue;
                }
                sprintf(file_path, "%s/%s",file, element->data);
                //on laisse vide les champs hash et mode car ils seront remplis plus tard
                appendWorkTree(new_wt, file_path, "", 0);
                element = element->next;
            }
            freeList(l);
            
            char* hash = saveWorkTree(new_wt, file);
            free(wt->tab[i].hash);
            wt->tab[i].hash = hash;

            freeWorkTree(new_wt);
        }
        wt->tab[i].mode = getChmod(file);
    }
    

    
    printf("---- Saving WorkTree at %s\n",path);
    return blobWorkTree(wt);
}

void restoreWorkTree(WorkTree* wt, char* path) {
    char cmd[WF_STR_SIZE];
    char* hash_path;

    printf("Restoring directory %s\n",path);
    sprintf(cmd, "mkdir -p %s",path);
    system(cmd);
    for (int i = 0; i < wt->n; i++) {
        hash_path = hashToPath(wt->tab[i].hash);

        //printf("Searching dir %s\n", rep);

        //si le fichier existe
        if (access(hash_path, F_OK) == 0) {
            printf("\tRestoring file %s from %s\n",wt->tab[i].name,hash_path);
            cp(wt->tab[i].name, hash_path);
            setMode(wt->tab[i].mode, wt->tab[i].name);
        } else {
            strcat(hash_path, ".t");
            //si c'est plutot un worktree au lieu d'un fichier regulier
            if (access(hash_path, F_OK) == 0)  {
                WorkTree* new_wt = ftwt(hash_path);
                if (new_wt == NULL) 
                    fprintf(stderr,"! Erreur de lecture du work tree %s\n",hash_path);
                restoreWorkTree(new_wt, wt->tab[i].name);
                freeWorkTree(new_wt);
            } else {
                fprintf(stderr,"! File %s does not exists\n", hash_path);
            }
        }

        free(hash_path);
    }

    printf("Directory %s restored successfully\n",path);
}