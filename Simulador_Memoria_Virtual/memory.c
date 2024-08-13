#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "memory.h"

Memory* create_memory(int max_size){
    Memory* m = (Memory*)malloc(sizeof(Memory));
    m->first = NULL;
    m->last = NULL;
    m->size = 0;
    m->max_size = max_size;
    return m;
}

Page* create_page(int addr){
    Page* p = (Page*)malloc(sizeof(Page));
    p->addr = addr;
    for(int i = 0; i < 3; i++){
        p->values[i] = 0;
    }
    p->values[1] = time(0);
    p->next = NULL;
    return p;
}

int insert(Memory* m, Page* p){
    if(m->size < m->max_size){
        if(m->size == 0){
            m->first = p;
            m->last = p;
            m->size++;
        }else{
            m->last->next = p;
            p->next = NULL;
            m->last = p;
            m->size++;
        }
        return 1;
    }
    return 0;
}

void remove_from_memory(Memory* m, int addr){
    if(m->size == 0) return;
    Page* current = m->first;
    Page* aux;
    while(current != NULL){
        if(current->addr == addr){
            free(current->values);
            m->size--;
        }
        current = current->next;
    }
}

void destroy(Memory* m){
    for(int i = 0; i < m->size; i++){
        remove_from_memory(m, m->last->addr);
    }
    free(m);
}

Page* search(Memory* m, int addr){
    if(m->size == 0)return NULL;
    Page* current = m->first;
    while(current != NULL){
        if(current->addr == addr){
            current->values[0]++;
            current->values[2] = time(0);
            return current;
        } 
        current = current->next;
    }
    return NULL;
}

void move_to_first(Memory* m, int addr){
    if(m->size == 1 || m->size == 2) return;
    Page* current = m->first;
    Page* aux;
    while(current->next != NULL){
        if(current->next->addr == addr){
            aux = current->next;
            if(current->next != m->last){
                current->next = current->next->next;
            }else{
                current->next = NULL;
                m->last = current;
            }
            aux->next = m->first;
            m->first = aux;
            return;
        }
        current = current->next;
    }
}

void move_first_to_last(Memory* m){
    if(m->size == 1 || m->size == 2) return;
    Page* aux = m->first;
    m->first = aux->next;
    m->last->next = aux;
    aux->next = NULL;
    m->last = aux;
}