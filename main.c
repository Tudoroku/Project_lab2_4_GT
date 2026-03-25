/*
*8. Magazin electronice

Creati o aplicatie care permite gestiunea stocului de produse intr-un magazin de electronice.
Fiecare produs are un identificator unic, tip (laptop, frigider, televizor,etc), producator, model,
pret, cantitate.
Aplicatia permite:
a) Adaugarea de noi produse. Daca produsul este deja in stoc trebuie actualizat cantitatea
b) Actualizare produse (modificare pret sau cantitate)
c) Stergere produs
d) Vizualizare produse din stoc, ordonat dupa pret, cantitate (crescator/descrescator)
e) Vizualizare lista de produse filtrate dupa un criteriu (producator, pret, cantitate)
*/
//
// Created by tudor on 3/10/2026.
//

#include "Repository/repo.h"
#include "Controller/service.h"
#include "UI/ui.h"
#include "Tests/tests.h"
#include <stdio.h>
#include <string.h>

static void load_sample_data(Service* service)
{
    service_add(service, 1, "laptop", "lenovo", "legion", 5200.0f, 3);
    service_add(service, 2, "televizor", "samsung", "qled", 3400.0f, 2);
    service_add(service, 3, "frigider", "beko", "neo", 2800.0f, 1);
    service_add(service, 4, "telefon", "apple", "iphone15", 4900.0f, 4);
    service_add(service, 5, "laptop", "asus", "vivobook", 3100.0f, 2);
}

int main(int argc, char** argv)
{
    run_all_tests();

    Repo repo;
    Service service;
    UI ui;

    if (!repo_init(&repo))
    {
        printf("Eroare: initializarea repository-ului a esuat.\n");
        return 1;
    }
    service_init(&service, &repo);
    ui_init(&ui, &service);

    if (argc > 1 && strcmp(argv[1], "--with-data") == 0)
    {
        load_sample_data(&service);
    }

    ui_run(&ui);
    service_destroy(&service);
    repo_destroy(&repo);
    return 0;
}
