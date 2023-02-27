#include "file_hash.h"

int hashFile(char* source, char* dest) {
	char cmd [STR_SIZE];
	
	sprintf(cmd,"cat %s | sha256sum > %s", source, dest);
	return system(cmd);
}

char* sha256file(char* file) {
	static char template [] = "tmpXXXXXX" ;
	char fname [400];
	char cmd [STR_SIZE];
	char *sha = (char*)malloc(STR_SIZE*sizeof(char));
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