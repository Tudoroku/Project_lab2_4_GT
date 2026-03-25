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

/*
 * Initializeaza lista.
 * pre: list != NULL, destroy_elem != NULL, copy_elem != NULL
 * post: lista are lungime 0 si capacitate initiala, daca alocarea reuseste
 * return: 1 daca initializarea a reusit, 0 altfel
 */
int list_init(List* list, ListDestroyFunction destroy_elem, ListCopyFunction copy_elem);

/*
 * Returneaza numarul de elemente din lista.
 */
int list_size(const List* list);

/*
 * Redimensioneaza memoria listei.
 * pre: list != NULL, opt este RESIZE_GROW sau RESIZE_SHRINK
 * post: capacitatea listei este modificata daca realocarea reuseste
 * return: 1 daca realocarea a reusit, 0 altfel
 */
int list_resize(List* list, int opt);

/*
 * Adauga un element la finalul listei.
 * pre: list != NULL, elem != NULL
 * post: elementul este adaugat in lista daca exista memorie suficienta
 * return: 1 daca s-a adaugat, 0 altfel
 */
int list_add(List* list, const void* elem);

/*
 * Returneaza adresa elementului de pe pozitia poz.
 * return: pointer la element daca pozitia este valida, NULL altfel
 */
void* list_get(const List* list, int poz);

/*
 * Inlocuieste elementul de pe pozitia poz.
 * pre: list != NULL, elem != NULL
 * post: elementul de pe pozitia data este inlocuit daca operatia reuseste
 * return: 1 daca inlocuirea a reusit, 0 altfel
 */
int list_replace(List* list, int poz, const void* elem);

/*
 * Sterge elementul de pe pozitia poz.
 * return: 1 daca s-a sters, 0 daca pozitia este invalida
 */
int list_remove(List* list, int poz);

/*
 * Copiaza elementele din src in dest.
 * post: dest contine o copie independenta a listei src daca operatia reuseste
 * return: 1 daca copierea a reusit, 0 altfel
 */
int list_copy(List* dest, const List* src);

/*
 * Elibereaza memoria alocata pentru lista.
 * post: lista devine goala si nu mai detine memorie alocata
 */
void list_destroy(List* list);

#ifdef LAB2_4_TESTING
void list_test_fail_next_malloc(void);
void list_test_fail_next_realloc(void);
#endif

#endif
