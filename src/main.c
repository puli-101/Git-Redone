#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include <assert.h>
#define STR_SIZE 500

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

int main() {
	char* sha = sha256file("src/main.c");
	printf("sha de src/main.c : %s\n", sha);
	free(sha);
	
	List* l = initList();
	insertFirst(l,buildCell("mot1"));
	insertFirst(l,buildCell("mot2"));
	char* str_list = ltos(l);
	
	assert(searchList(l,"mot1") != NULL);
	printf("%s\n", str_list);

	//free(l) <- detruire liste
	free(str_list);
	
	//test ftol
	List * leer;
	leer=ftol("test.txt");
	char buff[LIST_STR_SIZE];
	printf("%s \n", buff);
	ltof(leer,"test2.txt");

	return 0;
}
