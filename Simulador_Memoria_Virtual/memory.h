struct page{
    int addr;
    int values[3];
    struct page* next;
};

struct memory{
    struct page* first;
    struct page* last;
    int size;
    int max_size;
};

typedef struct page Page;
typedef struct memory Memory;

Memory* create_memory(int);
Page* create_page(int);
int insert(Memory*, Page*);
//void replace(Memory*, int);
void remove_from_memory(Memory*, int);
void destroy(Memory*);
Page* search(Memory*, int);
void move_to_first(Memory*, int);
void move_first_to_last(Memory*);