#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include "shellso.h"

void main(int argc, char** argv){
    char* prompt = "$ ";
    char text[512];

    //printf("%d\n", argc);
    if(argc > 1){
        //printf("%s\n", text);
        caminhar_nos_comandos(argv[1]);
        return;
    }

    do{
        printf("%s ", prompt);
        if(fgets(text, 100, stdin) == NULL) break;
        text[strcspn(text, "\n")] = 0;
        //if(strcmp(text, "\x04") == 0) break;
        if(strcmp(text, "fim") == 0) break;
        caminhar_nos_comandos(text);
    }while(strcmp(text, "fim\n") != 0 || (text[0] = getchar()) == EOF);
}