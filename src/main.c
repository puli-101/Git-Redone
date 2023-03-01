#include "list.h"
#include "file_hash.h"
#define STR_SIZE 500

int main() {
	//test de sha256 file
	char* sha = sha256file("src/main.c");
	printf("sha de src/main.c : %s\n", sha);
	free(sha);
	
	//test creation de liste
	List* l = initList();
	insertFirst(l,buildCell("mot1"));
	insertFirst(l,buildCell("mot2"));
	char* str_list = ltos(l);
	
	if (searchList(l,"mot1") != NULL) {
		printf("mot 1 found");
	} else {
		fprintf(stderr,"Error while searching for %s\n","mot1");
	}
	printf("%s\n", str_list);

	ltof(l,"tests/test_ltof.txt");

	freeList(l);
	free(str_list);

	//test creation d'instantanee
	l = listdir(".");
	str_list = ltos(l);
	printf("Current dir : %s\n",str_list);
	blobFile("src/main.c");

	free(str_list);
	freeList(l);

	//test ftol
	l = ftol("tests/test.txt");
	str_list = ltos(l);
	printf("%s\n",str_list);
	free(str_list);
	freeList(l);
	return 0;
}
