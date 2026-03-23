//
// Created by tudor on 3/10/2026.
//

#ifndef LAB2_4_UI_H
#define LAB2_4_UI_H

#include "../Controller/service.h"

typedef struct
{
    Service* service;
} UI;

/*
 * Initializeaza UI-ul.
 */
void ui_init(UI* ui, Service* s);

/*
 * Porneste aplicatia.
 */
void ui_run(UI* ui);

#endif //LAB2_4_UI_H