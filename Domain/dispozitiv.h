//
// Created by tudor on 3/10/2026.
//

#ifndef LAB2_4_DISPOZITIV_H
#define LAB2_4_DISPOZITIV_H


typedef struct
{
    int ID;
    char type[50];
    char prod[50];
    char model[50];
    float price;
    int quant;
}Dispozitiv;

/*
 * Returneaza id-ul unui dispozitiv.
 */
int get_id(const Dispozitiv * d);

/*
 * Returneaza cantitatea unui dispozitiv.
 */
int get_quant(const Dispozitiv * d);

/*
 * Returneaza pretul unui dispozitiv.
 */
float get_price(const Dispozitiv * d);

/*
 * Returneaza modelul unui dispozitiv.
 */
const char* get_model(const Dispozitiv * d);

/*
 * Returneaza producatorul unui dispozitiv.
 */
const char* get_prod(const Dispozitiv * d);

/*
 * Returneaza tipul unui dispozitiv.
 */
const char* get_type(const Dispozitiv * d);

/*
 * Creeaza un dispozitiv pe baza campurilor date.
 * return: dispozitivul creat
 */
Dispozitiv create_dispozitiv(int id, const char* type, const char* prod, const char* model, float price, int quant);

/*
 * Seteaza pretul unui dispozitiv.
 */
void set_price(Dispozitiv* d, float price);

/*
 * Seteaza cantitatea unui dispozitiv.
 */
void set_quant(Dispozitiv* d, int quant);
#endif //LAB2_4_DISPOZITIV_H
