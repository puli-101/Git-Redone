#ifndef FILE_HASH_H
#define FILE_HASH_H

#include "list.h"
#define HASH_STR_SIZE 300

/** Ecrit le hash du fichier source dans le fichier dest */
int hashFile(char* source, char* dest);

/** On alloue et retourne le hash du fichier file sous forme d'une chaine de caracteres */
char* sha256file(char* file);

/** Renvoie 1 si le fichier file exite, 0 a cas contraire.*/
int file_exists(char *file);

/** Copie le contenu du fichier from au fichier to*/
void cp(char *to, char *from);

/** A partir du hash on renvoie l'adresse du fichier correspondant au hash */
char* hashToPath(char* hash);

/** Va créer une copie du fichier file nommee d'apres son hash. Elle est rangee dans un repertoire 
 * nommee avec les deux premiers caracteres du hash.
*/
void blobFile(char* file);

/** Alloue et renvoie la direction d'un fichier à partir de son hash et elle lui concatène extension. */
char* hashToPathExtension(char* hash, char* extension);

#endif