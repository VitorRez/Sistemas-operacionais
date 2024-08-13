#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Memory** create_page_table(unsigned long int num_addr, unsigned long int num_pages){
    Memory** m = (Memory**)malloc(num_addr*sizeof(Memory*));
    for(int i = 0; i <= num_addr; i++){
        m[i] = create_memory(num_pages);
    }
    return m;
}

int insert_page(unsigned long int addr, unsigned long int size, Memory** m){
    unsigned long int pos = addr % size;
    Page* p = create_page(addr);
    int control = insert(m[pos], p);
    return control;
}

Page* search_page(unsigned long int addr, unsigned long int size, Memory** m, char* strategy){
    unsigned long int pos = addr % size;
    Page* p = search(m[pos], addr);
    if(strcmp(strategy, "lru") && p != NULL){
        move_to_first(m[pos], addr);
    }
    return p;
}

void destroy_page_table(Memory** m, unsigned long int num_addr){
    for(int i = 0; i < num_addr; i++){
        destroy(m[i]);
    }
    free(m);
}

int swap(Memory** m, unsigned long int addr, unsigned long int size, char* strategy, Page* p, int bit){
    unsigned long int pos = addr % size;
    if(strcmp(strategy, "lru") == 0){
        if(m[pos]->last->values[1] == 1) bit++;
        m[pos]->last->addr = p->addr;
        m[pos]->last->values[0] = p->values[0];
        m[pos]->last->values[1] = p->values[1];
        m[pos]->last->values[2] = p->values[2];
    }
    if(strcmp(strategy, "nru") == 0){
        int c0[size];
        int c1[size];
        int c2[size];
        int c3[size];
        int i0 = 0, i1 = 0, i2 = 0, i3 = 0;
        Page* current = m[pos]->first;
        while(current != NULL){
            if(current->values[0] == 0 && current->values[0] == 0){
                c0[i0] = current->addr;
                i0++;
            }
            if(current->values[0] == 0 && current->values[0] != 0){
                c1[i1] = current->addr;
                i1++;
            }
            if(current->values[0] =! 0 && current->values[0] == 0){
                c2[i2] = current->addr;
                i2++;
            }
            if(current->values[0] =! 0 && current->values[0] != 0){
                c3[i3] = current->addr;
                i3++;
            }
            current = current->next;
            if(i0 > 0){
                Page* p_s = search(m[pos], c0[0]);
                if(p_s->values[1] == 1) bit++;
                p_s->addr = p->addr;
                p_s->values[0] = p->values[0];
                p_s->values[1] = p->values[1];
                p_s->values[2] = p->values[2];
                return bit;
            }
            if(i1 > 0){
                Page* p_s = search(m[pos], c1[0]);
                if(p_s->values[1] == 1) bit++;
                p_s->addr = p->addr;
                p_s->values[0] = p->values[0];
                p_s->values[1] = p->values[1];
                p_s->values[2] = p->values[2];
                return bit;
            }
            if(i2 > 0){
                Page* p_s = search(m[pos], c2[0]);
                if(p_s->values[1] == 1) bit++;
                p_s->addr = p->addr;
                p_s->values[0] = p->values[0];
                p_s->values[1] = p->values[1];
                p_s->values[2] = p->values[2];
                return bit;
            }
            if(i3 > 0){
                Page* p_s = search(m[pos], c3[0]);
                if(p_s->values[1] == 1) bit++;
                p_s->addr = p->addr;
                p_s->values[0] = p->values[0];
                p_s->values[1] = p->values[1];
                p_s->values[2] = p->values[2];
                return bit;
            }
        } 
    }
    if(strcmp(strategy, "segunda_chance") == 0){
        int menor = time(0);
        int addr = 0;
        while(1){
            int menor = time(0);
            Page* current = m[pos]->first;
            while(current != NULL){
                if(current->values[2] <= menor){
                    addr = current->addr;
                    menor = current->values[2];
                    break;
                }
                current = current->next;
            }
            Page* p_s = search(m[pos], addr);
            if(p_s != m[pos]->first){
                if(p_s->values[1] == 1) bit++;
                p_s->addr = p->addr;
                p_s->values[0] = p->values[0];
                p_s->values[1] = p->values[1];
                p_s->values[2] = p->values[2];
                return bit;
            }else{
                m[pos]->first->values[2] = time(0);
            }
        }
    }
    return bit;
}