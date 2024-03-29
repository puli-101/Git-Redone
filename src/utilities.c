#include "utilities.h"

/** Renvoie 1 si les deux chaines de caracteres sont identiques. */
int equals(char* str1, char* str2) {
    return !strcmp(str1, str2);
}

/** Ecrit msg en la couleur color sur le fichier output.
 * Les couleurs disponibles sont  "red","yellow","green" et "blue".
 */
void print_color(FILE* output, char* msg, char* color) {
    char* values[] = {"\033[0;31m", "\033[0;33m", " \033[0;32m", "\033[0;34m"};
    char* value="\033[0m" ;
    if (equals(color, "red"))
        value = values[0];
    else if (equals(color, "yellow"))
        value = values[1];
    else if (equals(color, "green"))
        value = values[2];
    else if (equals(color, "blue"))
        value = values[3];
    fprintf(output, "%s%s\033[0m\n", value, msg);
}

/**
 * Fonction qui a partir d'un objet quelconque enregistre cet objet dans un fichier et
 * cree une copie d'une instantanee associee au contenu de l'objet et retourne le hash du fichier.
 * La fonction est utilisee dans blobWorkTree et blobCommit.
*/
char* blobContent(void* obj, char* extension, void (*toFile)(void*, char*)) {
    static char template [] = "tmpXXXXXX" ;
	char temp_obj_file [STR_SIZE - 100]; //fichier temporaire dans lequel on enregistrera une copie de l'instantanee
    char rep[3];
    char* instantanee;
	char cmd [STR_SIZE];
	char *sha;
    char* hash;
	
	strcpy (temp_obj_file , template) ;
	
	//On cree le fichier tmpX...
	int fd = mkstemp (temp_obj_file) ;

	if (fd == -1) {
        close(fd);
		return NULL;
	}

    close(fd);

    //On stocke la version string du WorkTree dans le temp_obj_file
    toFile(obj, temp_obj_file);

    //On recupere le hash du workfile stocke dans temp_obj_file
    sha = sha256file(temp_obj_file);

    //On recupere l'adresse correspondant au fichier instantanee
    hash = sha256file(temp_obj_file);
    instantanee = hashToPath(hash);
    free(hash);
    
    strcat(instantanee, extension);
    //On recupere le nom du repertoire
    rep[0] = instantanee[0];
    rep[1] = instantanee[1];
    rep[2] = '\0';
    //On cree le repertoire
    sprintf(cmd, "mkdir -p %s", rep);
    system(cmd);
    //On copie le fichier 'file' vers 'instantanee'
    cp(instantanee, temp_obj_file);
    free(instantanee);

    //On supprime le fichier temporaire
    sprintf(cmd,"rm %s",temp_obj_file);
	system(cmd);

	return sha;
}

/** Renvoie  1 si path est un fichier normal, 0 si non.*/
int is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

/** Renvoie 1 si path est un repertoire*/
int is_folder(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

/** Si le fichier path n'existe pas la fonction renvoie -1. Si non elle renvoie 1 si 
 * le fichier path est vide et 0 s'il n'est pas vide.
*/
int is_empty(const char* path) {
    struct stat stat_record;
    //si le fichier n'existe pas alors stat renvoie -1 (true)
    return stat(path, &stat_record) || stat_record.st_size <= 1;
}