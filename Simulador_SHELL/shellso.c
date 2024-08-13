#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "argv.h"
#include "auxiliar.h"

void executa_arquivo(char* comando, char** argv, int background){
    pid_t pid = fork();
    if(pid==0){
        execvp(comando, argv);
        exit(1);
    }else{
        if(background == 0){
            waitpid(pid,0,0);
        }
    }
}

int redirect_out(char* filename){
    int saved_stdout = dup(1);
    int out = open(filename, O_WRONLY | O_CREAT, S_IRUSR );
    dup2(out, 1);
    close(out);
    return(saved_stdout);
}

void return_out(int saved_stdout){
    dup2(saved_stdout, 1);
    close(saved_stdout);
}

int redirect_in(char* filename){
    int x = procura_arquivo(filename);
    if(x == 0){
        return -1;
    }else{
        int saved_stdin = dup(0);
        int in = open(filename, O_RDONLY);
        dup2(in, 0);
        close(in);
        return saved_stdin;
    }
}

void return_in(int saved_stdin){
    dup2(saved_stdin, 0);
    close(saved_stdin);
}

void Pipe(char*** argv, int argc, int background){
    int fd[2];
    pid_t pid;
    int in = 0;

    for(int i = 0; i < argc; i++){
        pipe(fd);
        pid = fork();
        if(pid == 0){
            dup2(in, 0);
            if(argv[i+1] != NULL){
                dup2(fd[1], 1);
            }
            close(fd[0]);
            execvp(argv[i][0], argv[i]);
            exit(1);
        }else{
            if(argv[i+1] == NULL){
                if(background == 0){
                    wait(NULL);
                }
            }else{
                wait(NULL);
            }
            close(fd[1]);
            in = fd[0];
        }
    }
}

void Pipeline(char** command_parsed, int e, int s, int size, int background){
    int p = 0;
    int pf = 0;
    int cont = 1;
    for(int i = 0; i < size; i++){
        if(strcmp(command_parsed[i], "|") == 0){
            if(pf == 0) pf = i;
            p = i;
            cont++;
        }
    }
    char** argv_f = cria_argv(e);
    preenche_argv(argv_f, command_parsed, e, 0);
    char** argv_l = cria_argv(s-p);
    preenche_argv(argv_l, command_parsed, (s-p-1), (p+1));
    char*** commands = (char***)malloc((cont+1)*sizeof(char**));
    commands[0] = argv_f;
    commands[cont-1] = argv_l;
    commands[cont] = NULL;
    if(cont > 1){
        cont = 1;
        if(pf != e){
            e = pf;
        }
        int x = e;
        for(int i = e+1; i < s; i++){
            if(strcmp(command_parsed[i], "|") == 0){
                char** argv = cria_argv(i-x);
                preenche_argv(argv, command_parsed, i-x-1, x+1);
                commands[cont] = argv;
                cont++;
                x = i;
            }
        }
    }
    Pipe(commands, cont+1, background);
    for(int i = 0; i <= cont; i++){
        delete_argv(commands[i]);
    }
    free(commands);
}

void caminhar_nos_comandos(char* command_line){
    int num_linhas = get_num_lines(command_line);
    char** command_parsed = processar_string(command_line, " ", num_linhas);
    int posicao_e = num_linhas;
    int posicao_p;
    int posicao_s = num_linhas;
    int e = 0;
    int s = 0;
    int p = 0;
    int saved_stdin = 0;
    int saved_stdout = 1;
    int background = 0;
    if(strcmp(command_parsed[num_linhas-1], "&") == 0){
        background = 1;
    }
    for(int i = 0; i < num_linhas; i++){
        if(strcmp(command_parsed[i], "<=") == 0){
            posicao_e = i;
            e = 1;
            int x = procura_arquivo(command_parsed[i+1]);
            if(x == 0) return;
            saved_stdin = redirect_in(command_parsed[i+1]);
            if(saved_stdin == -1) return;
        }
        if(strcmp(command_parsed[i], "|") == 0){
            posicao_p = i;
            p = 1;
        }
        if(strcmp(command_parsed[i], "=>") == 0){
            posicao_s = i;
            s = 1;
            saved_stdout = redirect_out(command_parsed[i+1]);
        }
    }
    if(p == 1){
        if(posicao_e == num_linhas){
            posicao_e = posicao_p;
        }
        Pipeline(command_parsed, posicao_e, posicao_s, num_linhas, background);
    }else{
        if(posicao_e == num_linhas){
            posicao_e = posicao_s;
        }
        char** argv = cria_argv(posicao_e);
        preenche_argv(argv, command_parsed, posicao_e, 0);
        executa_arquivo(command_parsed[0], argv, background); 
        delete_argv(argv);
    }
    if(e == 1){
        return_in(saved_stdin);
    }
    if(s == 1){
        return_out(saved_stdout);
    }
    free(command_parsed);
}