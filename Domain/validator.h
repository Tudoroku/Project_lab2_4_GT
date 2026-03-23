//
// Created by tudor on 3/10/2026.
//

#ifndef LAB2_4_VALIDATOR_H
#define LAB2_4_VALIDATOR_H

#include "dispozitiv.h"

/*
 * Valideaza campurile unui dispozitiv.
 * return:
 *  1 daca datele sunt valide
 *  0 altfel
 */
int validate_dispozitiv(int id, const char* type, const char* prod, const char* model, float price, int quant);

/*
 * Valideaza un dispozitiv deja creat.
 * return:
 *  1 daca dispozitivul este valid
 *  0 altfel
 */
int validate_dispozitiv_obj(const Dispozitiv* d);

#endif //LAB2_4_VALIDATOR_H