#include "service.h"

#include "../Domain/dispozitiv.h"
#include "../Domain/validator.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

static void service_destroy_repo_snapshot(void* elem)
{
    Repo* repo = (Repo*)elem;
    repo_destroy(repo);
    free(repo);
}

static void* service_copy_repo_snapshot(const void* elem)
{
    return repo_clone((const Repo*)elem);
}

static int service_store_undo_snapshot(Service* s)
{
    if (s == NULL || s->repo == NULL || s->undo_list.elems == NULL)
    {
        return 0;
    }

    return list_push_back(&s->undo_list, s->repo);
}

void service_init(Service* s, Repo* r)
{
    if (s == NULL)
    {
        return;
    }

    s->repo = r;
    if (!list_init(&s->undo_list, service_destroy_repo_snapshot, service_copy_repo_snapshot))
    {
        s->undo_list.elems = NULL;
        s->undo_list.lg = 0;
        s->undo_list.max_capacity = 0;
        s->undo_list.destroy_elem = NULL;
        s->undo_list.copy_elem = NULL;
    }
}

void service_destroy(Service* s)
{
    if (s == NULL)
    {
        return;
    }

    list_destroy(&s->undo_list);
    s->repo = NULL;
}

int service_add(Service* s, int id, const char* type, const char* prod, const char* model, float price, int quant)
{
    if (s == NULL || s->repo == NULL)
    {
        return -1;
    }

    if (!validate_dispozitiv(id, type, prod, model, price, quant))
    {
        return 0;
    }

    const int poz = repo_find_by_id(s->repo, id);
    if (!service_store_undo_snapshot(s))
    {
        return -1;
    }

    if (poz != -1)
    {
        const Dispozitiv* existent = repo_get(s->repo, poz);

        Dispozitiv d = create_dispozitiv(
            get_id(existent),
            get_type(existent),
            get_prod(existent),
            get_model(existent),
            get_price(existent),
            get_quant(existent) + quant
        );

        return repo_set(s->repo, poz, d) ? 1 : -1;
    }

    Dispozitiv d = create_dispozitiv(id, type, prod, model, price, quant);
    return repo_add(s->repo, d) ? 1 : -1;
}

int service_update(Service* s, int id, float new_price, int new_quant)
{
    if (s == NULL || s->repo == NULL)
    {
        return -1;
    }

    const int poz = repo_find_by_id(s->repo, id);
    if (poz == -1)
    {
        return 0;
    }

    const Dispozitiv* vechi = repo_get(s->repo, poz);
    Dispozitiv nou = create_dispozitiv(
        get_id(vechi),
        get_type(vechi),
        get_prod(vechi),
        get_model(vechi),
        new_price,
        new_quant
    );

    if (!validate_dispozitiv_obj(&nou))
    {
        return -1;
    }

    if (!service_store_undo_snapshot(s))
    {
        return -1;
    }

    return repo_set(s->repo, poz, nou) ? 1 : -1;
}

int service_delete(Service* s, int id)
{
    if (s == NULL || s->repo == NULL)
    {
        return 0;
    }

    if (repo_find_by_id(s->repo, id) == -1)
    {
        return 0;
    }

    if (!service_store_undo_snapshot(s))
    {
        return -1;
    }

    return repo_delete(s->repo, id);
}

int service_undo(Service* s)
{
    if (s == NULL || s->repo == NULL)
    {
        return -1;
    }

    const int history_size = list_size(&s->undo_list);
    if (history_size == 0)
    {
        return 0;
    }

    Repo* snapshot = (Repo*)list_get(&s->undo_list, history_size - 1);
    if (snapshot == NULL)
    {
        return -1;
    }

    if (!repo_replace(s->repo, snapshot))
    {
        return -1;
    }

    list_remove_at(&s->undo_list, history_size - 1);
    return 1;
}

int service_size(const Service* s)
{
    if (s == NULL || s->repo == NULL)
    {
        return 0;
    }

    return repo_size(s->repo);
}

int service_get_id(const Service* s, int poz)
{
    if (s == NULL || s->repo == NULL)
    {
        return -1;
    }

    const Dispozitiv* d = repo_get(s->repo, poz);
    return d == NULL ? -1 : get_id(d);
}

const char* service_get_type(const Service* s, int poz)
{
    if (s == NULL || s->repo == NULL)
    {
        return "";
    }

    const Dispozitiv* d = repo_get(s->repo, poz);
    return d == NULL ? "" : get_type(d);
}

const char* service_get_prod(const Service* s, int poz)
{
    if (s == NULL || s->repo == NULL)
    {
        return "";
    }

    const Dispozitiv* d = repo_get(s->repo, poz);
    return d == NULL ? "" : get_prod(d);
}

const char* service_get_model(const Service* s, int poz)
{
    if (s == NULL || s->repo == NULL)
    {
        return "";
    }

    const Dispozitiv* d = repo_get(s->repo, poz);
    return d == NULL ? "" : get_model(d);
}

float service_get_price(const Service* s, int poz)
{
    if (s == NULL || s->repo == NULL)
    {
        return -1.0f;
    }

    const Dispozitiv* d = repo_get(s->repo, poz);
    return d == NULL ? -1.0f : get_price(d);
}

int service_get_quant(const Service* s, int poz)
{
    if (s == NULL || s->repo == NULL)
    {
        return -1;
    }

    const Dispozitiv* d = repo_get(s->repo, poz);
    return d == NULL ? -1 : get_quant(d);
}

static int compare_by_field(const Dispozitiv* a, const Dispozitiv* b, int key)
{
    if (key == SERVICE_SORT_KEY_PRICE)
    {
        if (get_price(a) < get_price(b))
        {
            return -1;
        }
        if (get_price(a) > get_price(b))
        {
            return 1;
        }
    }
    else if (key == SERVICE_SORT_KEY_QUANTITY)
    {
        if (get_quant(a) < get_quant(b))
        {
            return -1;
        }
        if (get_quant(a) > get_quant(b))
        {
            return 1;
        }
    }

    return 0;
}

int service_sort(const Service* s, Dispozitiv* out, int max_count, int key, int order)
{
    if (s == NULL || s->repo == NULL || out == NULL || max_count < 0)
    {
        return -1;
    }
    if (key != SERVICE_SORT_KEY_PRICE && key != SERVICE_SORT_KEY_QUANTITY)
    {
        return -1;
    }
    if (order != SERVICE_SORT_ORDER_ASC && order != SERVICE_SORT_ORDER_DESC)
    {
        return -1;
    }

    const int n = repo_size(s->repo);
    const int count = (n < max_count) ? n : max_count;

    for (int i = 0; i < count; i++)
    {
        const Dispozitiv* d = repo_get(s->repo, i);
        if (d == NULL)
        {
            return -1;
        }
        out[i] = *d;
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            const int cmp = compare_by_field(&out[i], &out[j], key);
            const int should_swap = (order == SERVICE_SORT_ORDER_ASC && cmp > 0) ||
                                    (order == SERVICE_SORT_ORDER_DESC && cmp < 0);
            if (should_swap)
            {
                Dispozitiv tmp = out[i];
                out[i] = out[j];
                out[j] = tmp;
            }
        }
    }

    return count;
}

static int float_is_equal(float a, float b)
{
    return fabsf(a - b) < 0.0001f;
}

int service_filter_by_producer(const Service* s, const char* producer, Dispozitiv* out, int max_count)
{
    if (s == NULL || s->repo == NULL || producer == NULL || out == NULL || max_count < 0)
    {
        return -1;
    }

    int count = 0;
    const int n = repo_size(s->repo);
    for (int i = 0; i < n; i++)
    {
        const Dispozitiv* d = repo_get(s->repo, i);
        if (d == NULL)
        {
            return -1;
        }

        if (strcmp(get_prod(d), producer) == 0 && count < max_count)
        {
            out[count++] = *d;
        }
    }

    return count;
}

int service_filter_by_type(const Service* s, const char* type, Dispozitiv* out, int max_count)
{
    if (s == NULL || s->repo == NULL || type == NULL || out == NULL || max_count < 0)
    {
        return -1;
    }

    int count = 0;
    const int n = repo_size(s->repo);
    for (int i = 0; i < n; i++)
    {
        const Dispozitiv* d = repo_get(s->repo, i);
        if (d == NULL)
        {
            return -1;
        }

        if (strcmp(get_type(d), type) == 0 && count < max_count)
        {
            out[count++] = *d;
        }
    }

    return count;
}

int service_filter_by_price(const Service* s, float value, int cmp, Dispozitiv* out, int max_count)
{
    if (s == NULL || s->repo == NULL || out == NULL || max_count < 0)
    {
        return -1;
    }
    if (cmp != SERVICE_CMP_LESS && cmp != SERVICE_CMP_EQUAL && cmp != SERVICE_CMP_GREATER)
    {
        return -1;
    }

    int count = 0;
    const int n = repo_size(s->repo);
    for (int i = 0; i < n; i++)
    {
        const Dispozitiv* d = repo_get(s->repo, i);
        if (d == NULL)
        {
            return -1;
        }

        const float device_price = get_price(d);
        int matches = 0;
        if (cmp == SERVICE_CMP_LESS)
        {
            matches = device_price < value;
        }
        else if (cmp == SERVICE_CMP_EQUAL)
        {
            matches = float_is_equal(device_price, value);
        }
        else if (cmp == SERVICE_CMP_GREATER)
        {
            matches = device_price > value;
        }

        if (matches && count < max_count)
        {
            out[count++] = *d;
        }
    }

    return count;
}

int service_filter_by_quantity(const Service* s, int value, int cmp, Dispozitiv* out, int max_count)
{
    if (s == NULL || s->repo == NULL || out == NULL || max_count < 0)
    {
        return -1;
    }
    if (cmp != SERVICE_CMP_LESS && cmp != SERVICE_CMP_EQUAL && cmp != SERVICE_CMP_GREATER)
    {
        return -1;
    }

    int count = 0;
    const int n = repo_size(s->repo);
    for (int i = 0; i < n; i++)
    {
        const Dispozitiv* d = repo_get(s->repo, i);
        if (d == NULL)
        {
            return -1;
        }

        int matches = 0;
        if (cmp == SERVICE_CMP_LESS)
        {
            matches = get_quant(d) < value;
        }
        else if (cmp == SERVICE_CMP_EQUAL)
        {
            matches = get_quant(d) == value;
        }
        else if (cmp == SERVICE_CMP_GREATER)
        {
            matches = get_quant(d) > value;
        }

        if (matches && count < max_count)
        {
            out[count++] = *d;
        }
    }

    return count;
}
