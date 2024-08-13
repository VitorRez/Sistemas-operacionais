#include "memory.h"

Memory** create_page_table(unsigned long int, unsigned long int);
int insert_page(unsigned long int, unsigned long int, Memory**);
Page* search_page(unsigned long int, unsigned long int, Memory**, char*);
void destroy_page_table(Memory**, unsigned long int);
int swap(Memory**, unsigned long int, unsigned long int, char*, Page*, int);