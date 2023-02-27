#include "file_hash.h"

List* listdir(char* root_dir) {
    DIR * dp = opendir ( root_dir ) ;
    List* l = initList();

    struct dirent * ep ;
    if ( dp != NULL ) {
        while ((ep = readdir (dp)) != NULL ) {
            insertFirst(l, buildCell(ep->d_name));
        }
    }

    return l;
}

int file_exists(char *file) {
    DIR * dp = opendir ( "." ) ;
    
    struct dirent * ep ;
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) { 
            if (!strcmp(ep->d_name, file)) 
                return 1;
        }
    }

    return 0;
}

void cp(char *to, char *from) {
    FILE* src = fopen(from, "r");
    assert(src);
    FILE* dest = fopen(to, "w");
    assert(dest);

    char buff[LIST_STR_SIZE];
    while (fgets(buff, LIST_STR_SIZE, src)) {
        fputs(buff, dest);
    }

    fclose(src);
    fclose(dest);
}

int hashFile(char* source, char* dest) {
	char cmd [HASH_STR_SIZE];
	
	sprintf(cmd,"cat %s | sha256sum > %s", source, dest);
	return system(cmd);
}

char* sha256file(char* file) {
	static char template [] = "tmpXXXXXX" ;
	char fname [HASH_STR_SIZE - 100];
	char cmd [HASH_STR_SIZE];
	char *sha = (char*)malloc(HASH_STR_SIZE*sizeof(char));
	if (sha == NULL) 
		return NULL;
	
	strcpy (fname , template) ;
	
	//On cree le fichier tmpX...
	int fd = mkstemp (fname) ;

	//On stocke l'hash de file dans tmpX...
	if (fd == -1 || hashFile(file, fname) == -1) {
		free(sha);
		return NULL;
	}

	//On recupere l'hash dans tmpX...
	FILE* f = fopen(fname, "r");

	fscanf(f," %s ", sha);

	fclose(f);
	
	//On supprime tmpX...
	close(fd);
	sprintf(cmd,"rm %s",fname);
	system(cmd);
	return sha;
}

char* hashToPath(char* hash) {
    char* path = malloc(sizeof(char)*257);
    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';

    for (int  i = 3; i < 257; i++) {
        path[i] = hash[i - 1];
    } 

    return path;
}

void blobFile(char* file) {
    char* instantanee = hashToPath(sha256file(file));
    char rep[3];
    char cmd[HASH_STR_SIZE];
    rep[0] = instantanee[0];
    rep[1] = instantanee[1];
    rep[2] = '\0';
    sprintf(cmd, "mkdir %s", rep);
    cp(file, instantanee);
    free(instantanee);
}