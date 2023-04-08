#include "utilities.h"

int equals(char* str1, char* str2) {
    return !strcmp(str1, str2);
}

void print_color(FILE* output, char* msg, char* color) {
    char* values[] = {"\033[0;31m", "\033[0;33m", " \033[0;32m", "\033[0;34m"};
    char* value;
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