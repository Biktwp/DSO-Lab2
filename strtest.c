#include <stdio.h>
#include <string.h>

int main() {
    char path[132] = "/test/hola.txt";
    char *check = strtok(path, "/");
    while (check != NULL){
        printf("%s\n", check);
        check = strtok(NULL, "/");
    }
}
    