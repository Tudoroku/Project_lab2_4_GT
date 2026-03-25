//
// Created by tudor on 3/10/2026.
//

#ifndef LAB2_4_SERVICE_H
#define LAB2_4_SERVICE_H

#include "../Repository/repo.h"

typedef struct
{
    Repo* repo;
    List undo_list;
} Service;

#define SERVICE_SORT_KEY_PRICE 1
#define SERVICE_SORT_KEY_QUANTITY 2
#define SERVICE_SORT_ORDER_ASC 1
#define SERVICE_SORT_ORDER_DESC 2

#define SERVICE_CMP_LESS -1
#define SERVICE_CMP_EQUAL 0
#define SERVICE_CMP_GREATER 1

/*
 * Initializeaza service-ul.
 */
void service_init(Service* s, Repo* r);

/*
 * Elibereaza resursele auxiliare ale service-ului.
 */
void service_destroy(Service* s);

/*
 * Adauga un dispozitiv.
 * Daca exista deja un dispozitiv cu acelasi id, se actualizeaza cantitatea.
 *
 * return:
 *  1 daca operatia s-a realizat cu succes
 *  0 daca datele sunt invalide
 * -1 daca repo-ul este plin
 */
int service_add(Service* s, int id, const char* type, const char* prod, const char* model, float price, int quant);

/*
 * Modifica pretul si cantitatea unui dispozitiv existent.
 *
 * return:
 *  1 daca s-a modificat cu succes
 *  0 daca dispozitivul nu exista
 * -1 daca noile date sunt invalide
 */
int service_update(Service* s, int id, float new_price, int new_quant);

/*
 * Sterge un dispozitiv dupa id.
 *
 * return:
 *  1 daca s-a sters
 *  0 daca nu exista
 */
int service_delete(Service* s, int id);

/*
 * Reface ultima operatie de adaugare/modificare/stergere.
 *
 * return:
 *  1 daca undo a reusit
 *  0 daca nu exista operatii de anulat
 * -1 daca restaurarea a esuat
 */
int service_undo(Service* s);

/*
 * Returneaza numarul de dispozitive din stoc.
 */
int service_size(const Service* s);

/*
 * Returneaza id-ul dispozitivului de pe pozitia data.
 */
int service_get_id(const Service* s, int poz);

/*
 * Returneaza tipul dispozitivului de pe pozitia data.
 */
const char* service_get_type(const Service* s, int poz);

/*
 * Returneaza producatorul dispozitivului de pe pozitia data.
 */
const char* service_get_prod(const Service* s, int poz);

/*
 * Returneaza modelul dispozitivului de pe pozitia data.
 */
const char* service_get_model(const Service* s, int poz);

/*
 * Returneaza pretul dispozitivului de pe pozitia data.
 */
float service_get_price(const Service* s, int poz);

/*
 * Returneaza cantitatea dispozitivului de pe pozitia data.
 */
int service_get_quant(const Service* s, int poz);

/*
 * Copiaza dispozitivele intr-un buffer si le ordoneaza in functie de cheie si ordine.
 *
 * return:
 * >=0 numarul de elemente copiate
 *  -1 daca parametrii sunt invalizi
 */
int service_sort(const Service* s, Dispozitiv* out, int max_count, int key, int order);

/*
 * Filtrari dupa criterii. Rezultatele se copiaza in bufferul out.
 *
 * return:
 * >=0 numarul de elemente copiate
 *  -1 daca parametrii sunt invalizi
 */
int service_filter_by_producer(const Service* s, const char* producer, Dispozitiv* out, int max_count);
int service_filter_by_type(const Service* s, const char* type, Dispozitiv* out, int max_count);
int service_filter_by_price(const Service* s, float value, int cmp, Dispozitiv* out, int max_count);
int service_filter_by_quantity(const Service* s, int value, int cmp, Dispozitiv* out, int max_count);

#endif //LAB2_4_SERVICE_H
