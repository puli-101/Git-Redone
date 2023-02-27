#include "list.h"
#include "file_hash.h"
#define STR_SIZE 500

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
