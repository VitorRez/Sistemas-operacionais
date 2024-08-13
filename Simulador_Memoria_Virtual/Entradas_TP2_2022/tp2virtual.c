#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "page_table.h"

unsigned long int get_page(unsigned long int page_size, unsigned long int addr){
    unsigned long int tmp = page_size;
    unsigned long int s = 0;
    unsigned long int page;
    while(tmp>1){
        tmp = tmp >> 1;
        s++;
    }
    page = addr >> s;
    return page;
}

int main(int argc, char** argv){
    clock_t begin = clock();
    FILE* in = fopen(argv[2], "r");
    char mode;
    unsigned long int addr;
    unsigned long int page_size = atoi(argv[3])*2000;
    unsigned long int num_pages = atoi(argv[4])*2000;
    unsigned long int num_addr = num_pages/page_size;
    //printf("%ld\n", num_addr);
    Memory** m = create_page_table(num_addr, num_addr);
    unsigned long int cont_w = 0;
    unsigned long int cont_r = 0;
    unsigned long int cont_m = 0;
    unsigned long int cont_pf = 0;
    while(fscanf(in, "%lx %c", &addr, &mode) != EOF){
        //printf("%lx %c\n", addr, mode);
        int cont_m_aux = 0;
        unsigned long int page = get_page(page_size, (long int)addr);
        if(mode == 'W'){
            Page* p = search_page(page, num_addr, m, argv[1]);
            if(p == NULL){
                int control = insert_page(page, num_addr, m);
                if(control == 1){
                    cont_pf++;
                    cont_w++;
                }else{
                    Page* p_n = create_page(page);
                    cont_m_aux = swap(m, page, num_addr, argv[1], p_n, cont_m);
                    cont_m = cont_m + cont_m_aux;
                }
            }else{
                cont_w++;
                p->values[1] = 1;
            }
        }
        if(mode == 'R'){
            Page* p = search_page(page, num_addr, m, argv[1]);
            if(p == NULL){
                int control = insert_page(page, num_addr, m);
                if(control == 1){
                    cont_pf++;
                    cont_r++;
                }else{
                    Page* p_n = create_page(page);
                    cont_m_aux = swap(m, page, num_addr, argv[1], p_n, cont_m);
                    
                }
            }else{
                cont_r++;
                p->values[1] = 1;
            }
        }
    }
    printf("prompt> %s %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", argv[2]);
    printf("Tamanho da memoria: %s\n", argv[4]);
    printf("Tamanho das paginas: %s\n", argv[3]);
    printf("Tecnica de reposicao: %s\n", argv[1]);
    printf("Paginas lidas: %ld\n", cont_r);
    printf("Paginas escritas: %ld\n", cont_w);
    printf("Numero de acesso a memoria: %ld\n", cont_r+cont_w+cont_m+cont_pf);
    printf("Numero de page faults: %ld\n", cont_pf);
    printf("Numero de paginas sujas: %ld\n", cont_m);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tempo gasto: %f\n", time_spent);
    fclose(in);

}