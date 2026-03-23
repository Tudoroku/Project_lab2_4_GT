#ifndef LAB2_4_REPO_H
#define LAB2_4_REPO_H

#include "../Domain/dispozitiv.h"

#define INITIAL_CAPACITY 2
#define RESIZE_GROW 0
#define RESIZE_SHRINK 1

typedef struct
{
    Dispozitiv* elems;
    int lg;
    int max_capacity;
} Repo;

/*
 * Initializeaza repository-ul.
 * post: repo-ul are lungime 0 si capacitate initiala, daca alocarea reuseste
 * return: 1 daca initializarea a reusit, 0 daca alocarea a esuat
 */
int repo_init(Repo* r);

/*
 * Returneaza numarul de elemente din repo.
 */
int repo_size(const Repo* r);

/*
 * Redimensioneaza memoria repository-ului.
 * pre: r != NULL, opt este RESIZE_GROW sau RESIZE_SHRINK
 * post: capacitatea repo-ului este modificata daca realocarea reuseste
 * return: 1 daca realocarea a reusit, 0 altfel
 */
int repo_resize(Repo* r, int opt);

/*
 * Adauga un dispozitiv in repo.
 * pre: r != NULL
 * post: dispozitivul este adaugat in repo daca exista memorie suficienta
 * return: 1 daca s-a adaugat, 0 daca redimensionarea/alocarea a esuat
 */
int repo_add(Repo* r, Dispozitiv d);

/*
 * Cauta pozitia unui dispozitiv dupa id.
 * return: pozitia daca exista, -1 altfel
 */
int repo_find_by_id(const Repo* r, int id);

/*
 * Returneaza adresa dispozitivului de pe pozitia poz.
 * return: pointer la dispozitiv daca pozitia este valida, NULL altfel
 */
const Dispozitiv* repo_get(const Repo* r, int poz);

/*
 * Modifica dispozitivul de pe pozitia poz.
 * return: 1 daca a fost modificat, 0 daca pozitia este invalida
 */
int repo_set(Repo* r, int poz, Dispozitiv d);

/*
 * Sterge dispozitivul cu id dat.
 * return: 1 daca s-a sters, 0 daca nu exista
 */
int repo_delete(Repo* r, int id);

/*
 * Elibereaza memoria alocata pentru repo.
 * post: repo-ul devine gol si nu mai detine memorie alocata
 */
void repo_destroy(Repo* r);

#ifdef LAB2_4_TESTING
/*
 * Test helpers: force next allocation/reallocation in repo internals to fail.
 * Used to exercise error paths deterministically in unit tests.
 */
void repo_test_fail_next_malloc(void);
void repo_test_fail_next_realloc(void);
#endif

#endif
