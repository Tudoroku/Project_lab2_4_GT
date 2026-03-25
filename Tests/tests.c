#include "tests.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Controller/service.h"
#include "../Domain/dispozitiv.h"
#include "../Domain/validator.h"
#include "../Repository/list.h"
#include "../Repository/repo.h"

static int float_equal(float a, float b)
{
    return fabsf(a - b) < 0.0001f;
}

static void build_long_text(char* out, int  length)
{
    for (int i = 0; i < length; i++)
    {
        out[i] = (char)('a' + (i % 26));
    }
    out[length] = '\0';
}

static int fail_copy_int = 0;
static int fail_copy_int_malloc = 0;

static void destroy_int(void* elem)
{
    free(elem);
}

static void* copy_int(const void* elem)
{
    if (fail_copy_int || elem == NULL)
    {
        return NULL;
    }

    int* copy = fail_copy_int_malloc ? NULL : (int*)malloc(sizeof(int));
    if (copy == NULL) {
        fail_copy_int_malloc = 0;
        return NULL;
    }

    *copy = *(const int*)elem;
    return copy;
}

static void test_domain_create_and_getters(void)
{
    Dispozitiv d = create_dispozitiv(1, "laptop", "lenovo", "y700", 4500.5f, 3);

    assert(get_id(&d) == 1);
    assert(strcmp(get_type(&d), "laptop") == 0);
    assert(strcmp(get_prod(&d), "lenovo") == 0);
    assert(strcmp(get_model(&d), "y700") == 0);
    assert(float_equal(get_price(&d), 4500.5f));
    assert(get_quant(&d) == 3);
}

static void test_domain_setters(void)
{
    Dispozitiv d = create_dispozitiv(2, "tv", "samsung", "q60", 2999.0f, 5);

    set_price(&d, 2799.0f);
    set_quant(&d, 7);

    assert(float_equal(get_price(&d), 2799.0f));
    assert(get_quant(&d) == 7);
}

static void test_domain_defensive_guards(void)
{
    char very_long[120];
    build_long_text(very_long, 110);

    Dispozitiv d = create_dispozitiv(8, very_long, very_long, very_long, 99.0f, 1);
    assert(strlen(get_type(&d)) == 49);
    assert(strlen(get_prod(&d)) == 49);
    assert(strlen(get_model(&d)) == 49);

    Dispozitiv d_null_text = create_dispozitiv(9, NULL, NULL, NULL, 10.0f, 2);
    assert(strcmp(get_type(&d_null_text), "") == 0);
    assert(strcmp(get_prod(&d_null_text), "") == 0);
    assert(strcmp(get_model(&d_null_text), "") == 0);

    assert(get_id(NULL) == -1);
    assert(get_quant(NULL) == -1);
    assert(float_equal(get_price(NULL), -1.0f));
    assert(strcmp(get_type(NULL), "") == 0);
    assert(strcmp(get_prod(NULL), "") == 0);
    assert(strcmp(get_model(NULL), "") == 0);

    set_price(NULL, 100.0f);
    set_quant(NULL, 10);
}

static void test_validator(void)
{
    assert(validate_dispozitiv(1, "laptop", "lenovo", "y700", 4000.0f, 2) == 1);

    assert(validate_dispozitiv(0, "laptop", "lenovo", "y700", 4000.0f, 2) == 0);
    assert(validate_dispozitiv(1, "", "lenovo", "y700", 4000.0f, 2) == 0);
    assert(validate_dispozitiv(1, "laptop", NULL, "y700", 4000.0f, 2) == 0);
    assert(validate_dispozitiv(1, "laptop", "", "y700", 4000.0f, 2) == 0);
    assert(validate_dispozitiv(1, "laptop", "lenovo", NULL, 4000.0f, 2) == 0);
    assert(validate_dispozitiv(1, "laptop", "lenovo", "", 4000.0f, 2) == 0);
    assert(validate_dispozitiv(1, "laptop", "lenovo", "y700", 0.0f, 2) == 0);
    assert(validate_dispozitiv(1, "laptop", "lenovo", "y700", 4000.0f, -1) == 0);
    assert(validate_dispozitiv(1, NULL, "lenovo", "y700", 4000.0f, 1) == 0);

    Dispozitiv d = create_dispozitiv(3, "fridge", "beko", "bg1", 2000.0f, 4);
    assert(validate_dispozitiv_obj(&d) == 1);

    set_price(&d, -10.0f);
    assert(validate_dispozitiv_obj(&d) == 0);
}

static void test_validator_length_and_null_guards(void)
{
    char very_long[120];
    build_long_text(very_long, 110);

    assert(validate_dispozitiv(1, very_long, "lenovo", "model", 1000.0f, 2) == 0);
    assert(validate_dispozitiv(1, "laptop", very_long, "model", 1000.0f, 2) == 0);
    assert(validate_dispozitiv(1, "laptop", "lenovo", very_long, 1000.0f, 2) == 0);
    assert(validate_dispozitiv_obj(NULL) == 0);
}

static void test_repo_add_find_get_set_delete(void)
{
    Repo r;
    assert(repo_init(&r) == 1);

    Dispozitiv d1 = create_dispozitiv(10, "laptop", "hp", "pavilion", 3500.0f, 3);
    Dispozitiv d2 = create_dispozitiv(11, "tv", "sony", "bravia", 5000.0f, 2);
    Dispozitiv d3 = create_dispozitiv(12, "fridge", "beko", "neo", 2400.0f, 1);

    assert(repo_size(&r) == 0);
    assert(repo_add(&r, d1) == 1);
    assert(repo_add(&r, d2) == 1);
    assert(repo_add(&r, d3) == 1);
    assert(repo_size(&r) == 3);

    int p1 = repo_find_by_id(&r, 10);
    int p2 = repo_find_by_id(&r, 11);
    assert(p1 == 0);
    assert(p2 == 1);
    assert(repo_find_by_id(&r, 99) == -1);

    const Dispozitiv* first = repo_get(&r, p1);
    assert(strcmp(get_model(first), "pavilion") == 0);

    Dispozitiv replacement = create_dispozitiv(10, "laptop", "hp", "omen", 6000.0, 1);
    repo_set(&r, p1, replacement);
    assert(strcmp(get_model(repo_get(&r, p1)), "omen") == 0);

    assert(repo_delete(&r, 10) == 1);
    assert(repo_size(&r) == 2);
    assert(get_id(repo_get(&r, 0)) == 11);
    assert(get_id(repo_get(&r, 1)) == 12);

    assert(repo_delete(&r, 11) == 1);
    assert(repo_size(&r) == 1);
    assert(repo_delete(&r, 11) == 0);
    repo_destroy(&r);
}

static void test_list_generic_and_failure_paths(void)
{
    int value = 10;
    int other = 20;
    List list;
    List dest;

    assert(list_init(NULL, destroy_int, copy_int) == 0);
    assert(list_init(&list, NULL, copy_int) == 0);
    assert(list_init(&list, destroy_int, NULL) == 0);

    assert(list_init(&list, destroy_int, copy_int) == 1);
    assert(list_push_back(&list, &value) == 1);
    assert(*(int*)list_get(&list, 0) == 10);
    assert(list_set(&list, 0, &other) == 1);
    assert(*(int*)list_get(&list, 0) == 20);

    fail_copy_int = 1;
    assert(list_push_back(&list, &value) == 0);
    assert(list_set(&list, 0, &value) == 0);
    fail_copy_int = 0;

    fail_copy_int_malloc = 1;
    assert(list_push_back(&list, &value) == 0);

    assert(list_remove_at(&list, -1) == 0);
    assert(list_remove_at(NULL, 0) == 0);

    List broken_destroy = { NULL, 1, 0, NULL, NULL };
    list_destroy(&broken_destroy);

    List list_with_null_slot;
    assert(list_init(&list_with_null_slot, destroy_int, copy_int) == 1);
    assert(list_push_back(&list_with_null_slot, &value) == 1);
    destroy_int(list_with_null_slot.elems[0]);
    list_with_null_slot.elems[0] = NULL;
    list_destroy(&list_with_null_slot);

    assert(list_init(&dest, destroy_int, copy_int) == 1);
    assert(list_assign(NULL, &list) == 0);
    assert(list_assign(&dest, NULL) == 0);

    List broken_src = { NULL, 1, 0, NULL, NULL };
    assert(list_assign(&dest, &broken_src) == 0);

    list_test_fail_next_malloc();
    assert(list_assign(&dest, &list) == 0);

    fail_copy_int = 1;
    assert(list_assign(&dest, &list) == 0);
    fail_copy_int = 0;

    list_destroy(&dest);
    list_destroy(&list);
    list_destroy(NULL);
}

static void test_repo_full_capacity(void)
{
    Repo r;
    assert(repo_init(&r) == 1);

    for (int i = 0; i < r.max_capacity; i++)
    {
        Dispozitiv d = create_dispozitiv(i + 1, "laptop", "acer", "nitro", 4500.0f, 1);
        assert(repo_add(&r, d) == 1);
    }

    assert(repo_size(&r) == r.max_capacity);

    Dispozitiv extra = create_dispozitiv(1000, "phone", "apple", "iphone", 7000.0f, 2);
    assert(repo_add(&r, extra) == 1);
    assert(repo_size(&r) == INITIAL_CAPACITY + 1);
    assert(r.max_capacity > INITIAL_CAPACITY);

    repo_destroy(&r);
}

static void test_repo_resize_guards_and_shrink(void)
{
    Repo r;
    assert(repo_init(&r) == 1);

    assert(repo_resize(&r, 1) == 1);
    assert(r.max_capacity == INITIAL_CAPACITY);
    assert(repo_resize(&r, 42) == 0);
    assert(repo_get(&r, -1) == NULL);
    assert(repo_get(&r, 0) == NULL);

    Dispozitiv d1 = create_dispozitiv(1, "laptop", "acer", "nitro", 4000.0f, 1);
    Dispozitiv d2 = create_dispozitiv(2, "tv", "lg", "oled", 5000.0f, 1);
    Dispozitiv d3 = create_dispozitiv(3, "phone", "apple", "iphone", 7000.0f, 1);

    assert(repo_add(&r, d1) == 1);
    assert(repo_add(&r, d2) == 1);
    assert(repo_add(&r, d3) == 1);
    assert(r.max_capacity == 4);

    assert(repo_set(&r, -1, d1) == 0);
    assert(repo_set(&r, repo_size(&r), d1) == 0);

    assert(repo_delete(&r, 3) == 1);
    assert(repo_size(&r) == 2);
    assert(r.max_capacity == 4);
    assert(repo_delete(&r, 2) == 1);
    assert(repo_size(&r) == 1);
    assert(r.max_capacity == INITIAL_CAPACITY);

    repo_destroy(&r);
    assert(r.elems == NULL);
    assert(r.lg == 0);
    assert(r.max_capacity == 0);
}

static void test_repo_allocation_failure_paths(void)
{
    Repo r;

    list_test_fail_next_malloc();
    assert(repo_init(&r) == 0);
    assert(r.elems == NULL);
    assert(r.lg == 0);
    assert(r.max_capacity == 0);

    assert(repo_init(&r) == 1);
    Dispozitiv d1 = create_dispozitiv(1, "laptop", "acer", "nitro", 4000.0f, 1);
    Dispozitiv d2 = create_dispozitiv(2, "tv", "lg", "oled", 5000.0f, 1);
    Dispozitiv d3 = create_dispozitiv(3, "phone", "apple", "iphone", 7000.0f, 1);

    assert(repo_add(&r, d1) == 1);
    assert(repo_add(&r, d2) == 1);

    repo_test_fail_next_realloc();
    assert(repo_add(&r, d3) == 0);
    assert(repo_size(&r) == 2);
    assert(r.max_capacity == INITIAL_CAPACITY);

    repo_destroy(&r);
}

static void test_repo_clone_and_replace(void)
{
    Repo original;
    assert(repo_init(&original) == 1);
    assert(repo_add(&original, create_dispozitiv(1, "laptop", "lenovo", "legion", 5000.0f, 2)) == 1);
    assert(repo_add(&original, create_dispozitiv(2, "tv", "lg", "oled", 3500.0f, 1)) == 1);

    Repo* snapshot = repo_clone(&original);
    assert(snapshot != NULL);
    assert(repo_delete(&original, 1) == 1);
    assert(repo_size(&original) == 1);
    assert(repo_size(snapshot) == 2);

    assert(repo_replace(&original, snapshot) == 1);
    assert(repo_size(&original) == 2);
    assert(get_id(repo_get(&original, 0)) == 1);
    assert(get_id(repo_get(&original, 1)) == 2);

    repo_destroy(snapshot);
    free(snapshot);
    repo_destroy(&original);
}

static void test_repo_additional_failure_paths(void)
{
    Repo r;
    assert(repo_init(&r) == 1);
    assert(repo_add(&r, create_dispozitiv(1, "laptop", "acer", "nitro", 4000.0f, 1)) == 1);

    repo_test_fail_next_malloc();
    assert(repo_add(&r, create_dispozitiv(2, "tv", "lg", "oled", 5000.0f, 1)) == 0);

    assert(repo_clone(NULL) == NULL);

    repo_test_fail_next_malloc();
    assert(repo_clone(&r) == NULL);

    list_test_fail_next_malloc();
    assert(repo_clone(&r) == NULL);

    Repo broken_repo = r;
    broken_repo.copy_elem = NULL;
    assert(repo_clone(&broken_repo) == NULL);

    repo_destroy(&r);
}

static void test_repo_null_guards(void)
{
    Dispozitiv d = create_dispozitiv(1, "laptop", "lenovo", "legion", 5000.0f, 1);

    assert(repo_init(NULL) == 0);
    assert(repo_size(NULL) == 0);
    assert(repo_resize(NULL, RESIZE_GROW) == 0);
    assert(repo_add(NULL, d) == 0);
    assert(repo_find_by_id(NULL, 1) == -1);
    assert(repo_get(NULL, 0) == NULL);
    assert(repo_set(NULL, 0, d) == 0);
    assert(repo_delete(NULL, 1) == 0);
    repo_destroy(NULL);
}

static void test_service_add_update_delete(void)
{
    Repo r;
    Service s;
    assert(repo_init(&r) == 1);
    service_init(&s, &r);

    assert(service_add(&s, 1, "laptop", "lenovo", "legion", 6000.0f, 2) == 1);
    assert(service_size(&s) == 1);
    assert(service_get_id(&s, 0) == 1);
    assert(strcmp(service_get_type(&s, 0), "laptop") == 0);
    assert(strcmp(service_get_prod(&s, 0), "lenovo") == 0);
    assert(strcmp(service_get_model(&s, 0), "legion") == 0);
    assert(service_get_id(&s, 99) == -1);
    assert(strcmp(service_get_type(&s, 99), "") == 0);
    assert(strcmp(service_get_prod(&s, 99), "") == 0);
    assert(strcmp(service_get_model(&s, 99), "") == 0);
    assert(float_equal(service_get_price(&s, 99), -1.0f));
    assert(service_get_quant(&s, 99) == -1);

    assert(service_add(&s, 1, "laptop", "lenovo", "legion", 9999.0f, 3) == 1);
    assert(service_size(&s) == 1);
    assert(service_get_quant(&s, 0) == 5);
    assert(float_equal(service_get_price(&s, 0), 6000.0f));

    assert(service_update(&s, 1, 5800.0f, 4) == 1);
    assert(float_equal(service_get_price(&s, 0), 5800.0f));
    assert(service_get_quant(&s, 0) == 4);

    assert(service_update(&s, 1, -100.0f, 4) == -1);
    assert(service_update(&s, 999, 1000.0f, 1) == 0);

    assert(service_delete(&s, 1) == 1);
    assert(service_delete(&s, 1) == 0);
    assert(service_size(&s) == 0);
    service_destroy(&s);
    repo_destroy(&r);
}

static void test_service_add_invalid_and_dynamic_repo(void)
{
    Repo r;
    Service s;
    assert(repo_init(&r) == 1);
    service_init(&s, &r);

    assert(service_add(&s, -1, "laptop", "lenovo", "legion", 6000.0f, 2) == 0);

    const int initial_capacity = r.max_capacity;
    for (int i = 0; i < initial_capacity + 5; i++)
    {
        assert(service_add(&s, i + 1, "laptop", "acer", "nitro", 4000.0f, 1) == 1);
    }

    assert(service_size(&s) == initial_capacity + 5);
    assert(r.max_capacity >= service_size(&s));
    service_destroy(&s);
    repo_destroy(&r);
}

static void test_service_add_resize_failure(void)
{
    Repo r;
    Service s;
    assert(repo_init(&r) == 1);
    service_init(&s, &r);

    assert(service_add(&s, 1, "laptop", "acer", "nitro", 4000.0f, 1) == 1);
    assert(service_add(&s, 2, "tv", "lg", "oled", 5000.0f, 1) == 1);

    repo_test_fail_next_realloc();
    assert(service_add(&s, 3, "phone", "apple", "iphone", 7000.0f, 1) == -1);
    assert(service_size(&s) == 2);
    service_destroy(&s);
    repo_destroy(&r);
}

static void test_service_sorting(void)
{
    Repo r;
    Service s;
    assert(repo_init(&r) == 1);
    service_init(&s, &r);

    assert(service_add(&s, 1, "laptop", "lenovo", "legion", 3000.0f, 1) == 1);
    assert(service_add(&s, 2, "tv", "lg", "c1", 1500.0f, 7) == 1);
    assert(service_add(&s, 3, "fridge", "beko", "neo", 2500.0f, 4) == 1);

    Dispozitiv sorted[3];

    assert(service_sort(&s, sorted, 3, SERVICE_SORT_KEY_PRICE, SERVICE_SORT_ORDER_ASC) == 3);
    assert(get_id(&sorted[0]) == 2);
    assert(get_id(&sorted[1]) == 3);
    assert(get_id(&sorted[2]) == 1);

    assert(service_sort(&s, sorted, 3, SERVICE_SORT_KEY_PRICE, SERVICE_SORT_ORDER_DESC) == 3);
    assert(get_id(&sorted[0]) == 1);
    assert(get_id(&sorted[1]) == 3);
    assert(get_id(&sorted[2]) == 2);

    assert(service_sort(&s, sorted, 3, SERVICE_SORT_KEY_QUANTITY, SERVICE_SORT_ORDER_ASC) == 3);
    assert(get_id(&sorted[0]) == 1);
    assert(get_id(&sorted[1]) == 3);
    assert(get_id(&sorted[2]) == 2);

    assert(service_sort(&s, sorted, 3, SERVICE_SORT_KEY_QUANTITY, SERVICE_SORT_ORDER_DESC) == 3);
    assert(get_id(&sorted[0]) == 2);
    assert(get_id(&sorted[1]) == 3);
    assert(get_id(&sorted[2]) == 1);

    assert(service_sort(&s, sorted, 3, 99, SERVICE_SORT_ORDER_ASC) == -1);
    assert(service_sort(&s, sorted, 3, SERVICE_SORT_KEY_PRICE, 99) == -1);

    assert(service_add(&s, 4, "tablet", "samsung", "a9", 3000.0f, 2) == 1);
    Dispozitiv sorted_equal_price[4];
    assert(service_sort(&s, sorted_equal_price, 4, SERVICE_SORT_KEY_PRICE, SERVICE_SORT_ORDER_ASC) == 4);

    service_destroy(&s);
    repo_destroy(&r);
}

static void test_service_filtering(void)
{
    Repo r;
    Service s;
    assert(repo_init(&r) == 1);
    service_init(&s, &r);

    assert(service_add(&s, 1, "laptop", "lenovo", "legion", 3000.0f, 2) == 1);
    assert(service_add(&s, 2, "tv", "hp", "x1", 4500.0f, 5) == 1);
    assert(service_add(&s, 3, "fridge", "lenovo", "cool", 1500.0f, 1) == 1);
    assert(service_add(&s, 4, "monitor", "asus", "pro", 4500.0f, 7) == 1);

    Dispozitiv filtered[4];

    assert(service_filter_by_producer(&s, "lenovo", filtered, 4) == 2);
    assert(get_id(&filtered[0]) == 1);
    assert(get_id(&filtered[1]) == 3);

    assert(service_filter_by_type(&s, "tv", filtered, 4) == 1);
    assert(get_id(&filtered[0]) == 2);

    assert(service_filter_by_price(&s, 3000.0f, SERVICE_CMP_GREATER, filtered, 4) == 2);
    assert(get_id(&filtered[0]) == 2);
    assert(get_id(&filtered[1]) == 4);

    assert(service_filter_by_price(&s, 4500.0f, SERVICE_CMP_EQUAL, filtered, 4) == 2);
    assert(get_id(&filtered[0]) == 2);
    assert(get_id(&filtered[1]) == 4);

    assert(service_filter_by_price(&s, 3000.0f, SERVICE_CMP_LESS, filtered, 4) == 1);
    assert(get_id(&filtered[0]) == 3);

    assert(service_filter_by_quantity(&s, 3, SERVICE_CMP_LESS, filtered, 4) == 2);
    assert(get_id(&filtered[0]) == 1);
    assert(get_id(&filtered[1]) == 3);

    assert(service_filter_by_quantity(&s, 5, SERVICE_CMP_EQUAL, filtered, 4) == 1);
    assert(get_id(&filtered[0]) == 2);

    assert(service_filter_by_quantity(&s, 5, SERVICE_CMP_GREATER, filtered, 4) == 1);
    assert(get_id(&filtered[0]) == 4);

    assert(service_filter_by_price(&s, 1.0f, 99, filtered, 4) == -1);
    assert(service_filter_by_quantity(&s, 1, 99, filtered, 4) == -1);
    service_destroy(&s);
    repo_destroy(&r);
}

static void test_service_multi_undo(void)
{
    Repo r;
    Service s;
    assert(repo_init(&r) == 1);
    service_init(&s, &r);

    assert(service_add(&s, 1, "laptop", "lenovo", "legion", 5000.0f, 2) == 1);
    assert(service_add(&s, 2, "tv", "lg", "oled", 3500.0f, 1) == 1);
    assert(service_update(&s, 1, 4500.0f, 4) == 1);
    assert(service_delete(&s, 2) == 1);
    assert(service_size(&s) == 1);

    assert(service_undo(&s) == 1);
    assert(service_size(&s) == 2);
    assert(repo_find_by_id(&r, 2) != -1);

    assert(service_undo(&s) == 1);
    assert(float_equal(service_get_price(&s, 0), 5000.0f));
    assert(service_get_quant(&s, 0) == 2);

    assert(service_undo(&s) == 1);
    assert(service_size(&s) == 1);
    assert(repo_find_by_id(&r, 2) == -1);

    assert(service_undo(&s) == 1);
    assert(service_size(&s) == 0);
    assert(service_undo(&s) == 0);

    service_destroy(&s);
    repo_destroy(&r);
}

static void test_service_undo_failure_paths(void)
{
    Repo r;
    Service s;
    Repo* broken_snapshot;

    assert(repo_init(&r) == 1);

    list_test_fail_next_malloc();
    service_init(&s, &r);
    assert(s.undo_list.elems == NULL);
    assert(service_add(&s, 1, "laptop", "lenovo", "legion", 5000.0f, 1) == -1);

    assert(repo_add(&r, create_dispozitiv(1, "laptop", "lenovo", "legion", 5000.0f, 1)) == 1);
    assert(service_update(&s, 1, 4500.0f, 2) == -1);
    assert(service_delete(&s, 1) == -1);
    service_destroy(NULL);
    service_destroy(&s);

    service_init(&s, &r);
    s.undo_list.elems[0] = NULL;
    s.undo_list.lg = 1;
    assert(service_undo(&s) == -1);
    s.undo_list.lg = 0;

    broken_snapshot = (Repo*)malloc(sizeof(Repo));
    assert(broken_snapshot != NULL);
    *broken_snapshot = (Repo){ NULL, 1, 0, NULL, NULL };
    s.undo_list.elems[0] = broken_snapshot;
    s.undo_list.lg = 1;
    assert(service_undo(&s) == -1);
    s.undo_list.lg = 0;

    service_destroy(&s);
    repo_destroy(&r);
}

static void test_service_null_guards(void)
{
    Dispozitiv out[1];
    Repo r;
    Service s;

    assert(repo_init(&r) == 1);
    service_init(&s, &r);
    service_init(NULL, &r);

    assert(service_add(NULL, 1, "laptop", "lenovo", "legion", 3000.0f, 1) == -1);
    assert(service_update(NULL, 1, 1000.0f, 2) == -1);
    assert(service_delete(NULL, 1) == 0);
    assert(service_undo(NULL) == -1);
    assert(service_size(NULL) == 0);
    assert(service_get_id(NULL, 0) == -1);
    assert(strcmp(service_get_type(NULL, 0), "") == 0);
    assert(strcmp(service_get_prod(NULL, 0), "") == 0);
    assert(strcmp(service_get_model(NULL, 0), "") == 0);
    assert(float_equal(service_get_price(NULL, 0), -1.0f));
    assert(service_get_quant(NULL, 0) == -1);
    assert(service_sort(NULL, out, 1, SERVICE_SORT_KEY_PRICE, SERVICE_SORT_ORDER_ASC) == -1);
    assert(service_sort(NULL, NULL, 0, SERVICE_SORT_KEY_PRICE, SERVICE_SORT_ORDER_ASC) == -1);
    assert(service_filter_by_producer(NULL, "lenovo", out, 1) == -1);
    assert(service_filter_by_type(NULL, "laptop", out, 1) == -1);
    assert(service_filter_by_price(NULL, 1000.0f, SERVICE_CMP_EQUAL, out, 1) == -1);
    assert(service_filter_by_quantity(NULL, 1, SERVICE_CMP_EQUAL, out, 1) == -1);

    service_destroy(&s);
    repo_destroy(&r);
}

static void test_service_repo_corrupted_guards(void)
{
    Repo broken_repo = { NULL, 1, 0, NULL, NULL };
    Service s;
    Dispozitiv out[2];

    service_init(&s, &broken_repo);

    assert(service_sort(&s, out, 1, SERVICE_SORT_KEY_PRICE, SERVICE_SORT_ORDER_ASC) == -1);
    assert(service_filter_by_producer(&s, "x", out, 1) == -1);
    assert(service_filter_by_type(&s, "x", out, 1) == -1);
    assert(service_filter_by_price(&s, 100.0f, SERVICE_CMP_GREATER, out, 1) == -1);
    assert(service_filter_by_quantity(&s, 1, SERVICE_CMP_GREATER, out, 1) == -1);
    service_destroy(&s);
}

void run_all_tests(void)
{
    test_domain_create_and_getters();
    test_domain_setters();
    test_domain_defensive_guards();
    test_validator();
    test_validator_length_and_null_guards();
    test_list_generic_and_failure_paths();
    test_repo_add_find_get_set_delete();
    test_repo_full_capacity();
    test_repo_resize_guards_and_shrink();
    test_repo_allocation_failure_paths();
    test_repo_clone_and_replace();
    test_repo_additional_failure_paths();
    test_repo_null_guards();
    test_service_add_update_delete();
    test_service_add_invalid_and_dynamic_repo();
    test_service_add_resize_failure();
    test_service_sorting();
    test_service_filtering();
    test_service_multi_undo();
    test_service_undo_failure_paths();
    test_service_null_guards();
    test_service_repo_corrupted_guards();

    printf("All tests passed.\n");
}
