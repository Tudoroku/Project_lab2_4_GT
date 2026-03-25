#include "repo.h"

#include <stdlib.h>

#ifdef LAB2_4_TESTING
static int repo_fail_next_malloc = 0;

static void* repo_malloc(size_t size)
{
    if (repo_fail_next_malloc)
    {
        repo_fail_next_malloc = 0;
        return NULL;
    }

    return malloc(size);
}
#else
#define repo_malloc malloc
#endif

static void repo_destroy_dispozitiv(void* elem)
{
    free(elem);
}

static void* repo_copy_dispozitiv(const void* elem)
{
    Dispozitiv* copy = (Dispozitiv*)repo_malloc(sizeof(Dispozitiv));
    if (copy == NULL)
    {
        return NULL;
    }

    *copy = *(const Dispozitiv*)elem;
    return copy;
}

int repo_init(Repo* r)
{
    return list_init(r, repo_destroy_dispozitiv, repo_copy_dispozitiv);
}

int repo_size(const Repo* r)
{
    return list_size(r);
}

int repo_resize(Repo* r, int opt)
{
    return list_resize(r, opt);
}

int repo_add(Repo* r, Dispozitiv d)
{
    return list_push_back(r, &d);
}

int repo_find_by_id(const Repo* r, int id)
{
    if (r == NULL || r->elems == NULL)
    {
        return -1;
    }

    for (int i = 0; i < r->lg; i++)
    {
        const Dispozitiv* d = (const Dispozitiv*)list_get(r, i);
        if (d != NULL && get_id(d) == id)
        {
            return i;
        }
    }

    return -1;
}

const Dispozitiv* repo_get(const Repo* r, int poz)
{
    return (const Dispozitiv*)list_get(r, poz);
}

int repo_set(Repo* r, int poz, Dispozitiv d)
{
    return list_set(r, poz, &d);
}

int repo_delete(Repo* r, int id)
{
    const int poz = repo_find_by_id(r, id);
    if (poz == -1)
    {
        return 0;
    }

    return list_remove_at(r, poz);
}

void repo_destroy(Repo* r)
{
    list_destroy(r);
}

Repo* repo_clone(const Repo* r)
{
    if (r == NULL)
    {
        return NULL;
    }

    Repo* clone = (Repo*)repo_malloc(sizeof(Repo));
    if (clone == NULL)
    {
        return NULL;
    }

    if (!repo_init(clone))
    {
        free(clone);
        return NULL;
    }

    if (!repo_replace(clone, r))
    {
        repo_destroy(clone);
        free(clone);
        return NULL;
    }

    return clone;
}

int repo_replace(Repo* dest, const Repo* src)
{
    return list_assign(dest, src);
}

#ifdef LAB2_4_TESTING
void repo_test_fail_next_malloc(void)
{
    repo_fail_next_malloc = 1;
}

void repo_test_fail_next_realloc(void)
{
    list_test_fail_next_realloc();
}
#endif
