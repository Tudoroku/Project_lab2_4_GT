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

int main()
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

    ui_run(&ui);
    repo_destroy(&repo);
    return 0;
}

