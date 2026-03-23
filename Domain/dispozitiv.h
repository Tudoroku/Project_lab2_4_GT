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

int get_id(const Dispozitiv * d);
int get_quant(const Dispozitiv * d);
float get_price(const Dispozitiv * d);
const char* get_model(const Dispozitiv * d);
const char* get_prod(const Dispozitiv * d);
const char* get_type(const Dispozitiv * d);
Dispozitiv create_dispozitiv(int id, const char* type, const char* prod, const char* model, float price, int quant);
void set_price(Dispozitiv* d, float price);
void set_quant(Dispozitiv* d, int quant);
#endif //LAB2_4_DISPOZITIV_H