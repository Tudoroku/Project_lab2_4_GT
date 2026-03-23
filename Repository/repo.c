//
// Created by tudor on 3/10/2026.
//

#include "repo.h"
#include <stdlib.h>

#ifdef LAB2_4_TESTING
static int repo_fail_next_malloc = 0;
static int repo_fail_next_realloc = 0;

void repo_test_fail_next_malloc(void)
{
    repo_fail_next_malloc = 1;
}

void repo_test_fail_next_realloc(void)
{
    repo_fail_next_realloc = 1;
}

static void* repo_malloc(size_t size)
{
    if (repo_fail_next_malloc)
    {
        repo_fail_next_malloc = 0;
        return NULL;
    }
    return malloc(size);
}

static void* repo_realloc(void* ptr, size_t size)
{
    if (repo_fail_next_realloc)
    {
        repo_fail_next_realloc = 0;
        return NULL;
    }
    return realloc(ptr, size);
}
#else
#define repo_malloc malloc
#define repo_realloc realloc
#endif

int repo_init(Repo* r)
{
    if (r == NULL)
    {
        return 0;
    }

    r->lg = 0;
    r->max_capacity = INITIAL_CAPACITY;
    r->elems = (Dispozitiv*)repo_malloc(r->max_capacity * sizeof(Dispozitiv));
    if (r->elems == NULL)
    {
        r->max_capacity = 0;
        r->lg = 0;
        return 0;
    }
    return 1;
}

int repo_size(const Repo* r)
{
    if (r == NULL)
    {
        return 0;
    }

    return r->lg;
}

int repo_resize(Repo *r, int opt)
{
    if (r == NULL || r->elems == NULL || r->max_capacity <= 0)
    {
        return 0;
    }

    int new_capacity;
    if (opt == RESIZE_GROW)
    {
        // if opt = 0, reallocate more space
        new_capacity = r->max_capacity * 2;
    }
    else if (opt == RESIZE_SHRINK)
    {
        // if opt = 1, reallocate less space
        new_capacity = r->max_capacity / 2;
        if (new_capacity < INITIAL_CAPACITY)
        {
            new_capacity = INITIAL_CAPACITY;
        }
    }
    else
    {
        return 0;
    }
    if (new_capacity == r->max_capacity)
    {
        return 1;
    }
    Dispozitiv* tmp = (Dispozitiv*)repo_realloc(r->elems, new_capacity * sizeof(Dispozitiv));
    if (tmp == NULL)
    {
        return 0;
    }
    r->elems = tmp;
    r->max_capacity = new_capacity;
    return 1;
}
int repo_add(Repo* r, Dispozitiv d)
{
    if (r == NULL || r->elems == NULL)
    {
        return 0;
    }

    if (r->lg == r->max_capacity)
    {
        if (!repo_resize(r, RESIZE_GROW))
        {
            return 0;
        }
    }

    r->elems[r->lg] = d;
    r->lg++;
    return 1;
}

int repo_find_by_id(const Repo* r, int id)
{
    if (r == NULL || r->elems == NULL)
    {
        return -1;
    }

    for (int i = 0; i < r->lg; i++)
    {
        if (get_id(&r->elems[i]) == id)
        {
            return i;
        }
    }
    return -1;
}

const Dispozitiv* repo_get(const Repo* r, int poz)
{
    if (r == NULL || r->elems == NULL)
    {
        return NULL;
    }

    if (poz < 0 || poz >= r->lg)
    {
        return NULL;
    }
    return &r->elems[poz];
}

int repo_set(Repo* r, int poz, Dispozitiv d)
{
    if (r == NULL || r->elems == NULL)
    {
        return 0;
    }

    if (poz < 0 || poz >= r->lg)
    {
        return 0;
    }

    r->elems[poz] = d;
    return 1;
}

int repo_delete(Repo* r, int id)
{
    if (r == NULL || r->elems == NULL)
    {
        return 0;
    }

    int poz = repo_find_by_id(r, id);
    if (poz == -1)
    {
        return 0;
    }
    for (int i = poz; i < r->lg - 1; i++)
    {
        r->elems[i] = r->elems[i + 1];
    }

    r->lg--;
    if (r->max_capacity > INITIAL_CAPACITY && r->lg <= r->max_capacity / 4)
    {
        repo_resize(r, RESIZE_SHRINK);
    }
    return 1;
}

void repo_destroy(Repo* r)
{
    if (r == NULL)
    {
        return;
    }

    free(r->elems);
    r->elems = NULL;
    r->lg = 0;
    r->max_capacity = 0;
}
