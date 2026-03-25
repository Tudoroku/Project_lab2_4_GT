#ifndef LAB2_4_LIST_H
#define LAB2_4_LIST_H

typedef void (*ListDestroyFunction)(void* elem);
typedef void* (*ListCopyFunction)(const void* elem);

typedef struct
{
    void** elems;
    int lg;
    int max_capacity;
    ListDestroyFunction destroy_elem;
    ListCopyFunction copy_elem;
} List;

#define INITIAL_CAPACITY 2
#define RESIZE_GROW 0
#define RESIZE_SHRINK 1

int list_init(List* list, ListDestroyFunction destroy_elem, ListCopyFunction copy_elem);
int list_size(const List* list);
int list_resize(List* list, int opt);
int list_push_back(List* list, const void* elem);
void* list_get(const List* list, int poz);
int list_set(List* list, int poz, const void* elem);
int list_remove_at(List* list, int poz);
int list_assign(List* dest, const List* src);
void list_destroy(List* list);

#ifdef LAB2_4_TESTING
void list_test_fail_next_malloc(void);
void list_test_fail_next_realloc(void);
#endif

#endif
