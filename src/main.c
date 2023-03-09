#include "list.h"
#include "file_hash.h"
#include "work_file.h"
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

	//work file
	WorkTree* wt = initWorkTree();
	appendWorkTree(wt,"src/main.c", "12415f53a12e12d", 666);
	appendWorkTree(wt,"src/file_hash.c", "15f53a12e1de193", 665);
	appendWorkTree(wt,"src/list.c", "2415f53a12e12de211", 655);
	char* str = wtts(wt);
	
	printf("work tree : \n%s", str);

	WorkTree* wt2 = stwt(str);
	free(str);
	str = wtts(wt2);
	printf("work tree 2 : \n%s", str);
	printf("work tree size : %d\n", wt2->n);

	//wttf(wt2,"tests/test_wttf.txt");

	WorkTree* wt3 = ftwt("tests/test_wttf.txt");
	free(str);
	str = wtts(wt3);
	printf("work tree 3 : \n%s", str);
	printf("work tree size : %d\n", wt3->n);

	free(str);
	str = blobWorkTree(wt3);
	printf("hash du wt3 : %s\n", str);

	freeWorkTree(wt);
	freeWorkTree(wt2);
	freeWorkTree(wt3);
	
	//test saveWorkTree
	WorkTree* wt4 = initWorkTree();
	appendWorkTree(wt4,"tests","",0);
	str = saveWorkTree(wt4,".");
	free(str);
	freeWorkTree(wt4);
	return 0;
}
