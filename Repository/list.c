#include "list.h"

#include <stdlib.h>

#ifdef LAB2_4_TESTING
static int list_fail_next_malloc = 0;
static int list_fail_next_realloc = 0;

void list_test_fail_next_malloc(void)
{
    list_fail_next_malloc = 1;
}

void list_test_fail_next_realloc(void)
{
    list_fail_next_realloc = 1;
}

static void* list_malloc(size_t size)
{
    if (list_fail_next_malloc)
    {
        list_fail_next_malloc = 0;
        return NULL;
    }

    return malloc(size);
}

static void* list_realloc(void* ptr, size_t size)
{
    if (list_fail_next_realloc)
    {
        list_fail_next_realloc = 0;
        return NULL;
    }

    return realloc(ptr, size);
}
#else
#define list_malloc malloc
#define list_realloc realloc
#endif

static void list_destroy_range(List* list, int from)
{
    if (list == NULL || list->elems == NULL || list->destroy_elem == NULL)
    {
        return;
    }

    for (int i = from; i < list->lg; i++)
    {
        if (list->elems[i] != NULL)
        {
            list->destroy_elem(list->elems[i]);
            list->elems[i] = NULL;
        }
    }
}

int list_init(List* list, ListDestroyFunction destroy_elem, ListCopyFunction copy_elem)
{
    if (list == NULL || destroy_elem == NULL || copy_elem == NULL)
    {
        return 0;
    }

    list->lg = 0;
    list->max_capacity = INITIAL_CAPACITY;
    list->destroy_elem = destroy_elem;
    list->copy_elem = copy_elem;
    list->elems = (void**)list_malloc((size_t)list->max_capacity * sizeof(void*));
    if (list->elems == NULL)
    {
        list->lg = 0;
        list->max_capacity = 0;
        list->destroy_elem = NULL;
        list->copy_elem = NULL;
        return 0;
    }

    return 1;
}

int list_size(const List* list)
{
    if (list == NULL)
    {
        return 0;
    }

    return list->lg;
}

int list_resize(List* list, int opt)
{
    if (list == NULL || list->elems == NULL || list->max_capacity <= 0)
    {
        return 0;
    }

    int new_capacity;
    if (opt == RESIZE_GROW)
    {
        new_capacity = list->max_capacity * 2;
    }
    else if (opt == RESIZE_SHRINK)
    {
        new_capacity = list->max_capacity / 2;
        if (new_capacity < INITIAL_CAPACITY)
        {
            new_capacity = INITIAL_CAPACITY;
        }
    }
    else
    {
        return 0;
    }

    if (new_capacity == list->max_capacity)
    {
        return 1;
    }

    void** tmp = (void**)list_realloc(list->elems, (size_t)new_capacity * sizeof(void*));
    if (tmp == NULL)
    {
        return 0;
    }

    list->elems = tmp;
    list->max_capacity = new_capacity;
    return 1;
}

int list_push_back(List* list, const void* elem)
{
    if (list == NULL || list->elems == NULL || elem == NULL || list->copy_elem == NULL)
    {
        return 0;
    }

    if (list->lg == list->max_capacity && !list_resize(list, RESIZE_GROW))
    {
        return 0;
    }

    void* copy = list->copy_elem(elem);
    if (copy == NULL)
    {
        return 0;
    }

    list->elems[list->lg++] = copy;
    return 1;
}

void* list_get(const List* list, int poz)
{
    if (list == NULL || list->elems == NULL || poz < 0 || poz >= list->lg)
    {
        return NULL;
    }

    return list->elems[poz];
}

int list_set(List* list, int poz, const void* elem)
{
    if (list == NULL || list->elems == NULL || elem == NULL || poz < 0 || poz >= list->lg)
    {
        return 0;
    }

    void* copy = list->copy_elem(elem);
    if (copy == NULL)
    {
        return 0;
    }

    if (list->destroy_elem != NULL && list->elems[poz] != NULL)
    {
        list->destroy_elem(list->elems[poz]);
    }

    list->elems[poz] = copy;
    return 1;
}

int list_remove_at(List* list, int poz)
{
    if (list == NULL || list->elems == NULL || poz < 0 || poz >= list->lg)
    {
        return 0;
    }

    if (list->destroy_elem != NULL && list->elems[poz] != NULL)
    {
        list->destroy_elem(list->elems[poz]);
    }

    for (int i = poz; i < list->lg - 1; i++)
    {
        list->elems[i] = list->elems[i + 1];
    }

    list->lg--;
    list->elems[list->lg] = NULL;

    if (list->max_capacity > INITIAL_CAPACITY && list->lg <= list->max_capacity / 4)
    {
        list_resize(list, RESIZE_SHRINK);
    }

    return 1;
}

int list_assign(List* dest, const List* src)
{
    if (dest == NULL || src == NULL || src->destroy_elem == NULL || src->copy_elem == NULL)
    {
        return 0;
    }

    List copy;
    if (!list_init(&copy, src->destroy_elem, src->copy_elem))
    {
        return 0;
    }

    for (int i = 0; i < src->lg; i++)
    {
        if (!list_push_back(&copy, src->elems[i]))
        {
            list_destroy(&copy);
            return 0;
        }
    }

    list_destroy(dest);
    *dest = copy;
    return 1;
}

void list_destroy(List* list)
{
    if (list == NULL)
    {
        return;
    }

    list_destroy_range(list, 0);
    free(list->elems);
    list->elems = NULL;
    list->lg = 0;
    list->max_capacity = 0;
    list->destroy_elem = NULL;
    list->copy_elem = NULL;
}
