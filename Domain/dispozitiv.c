//
// Created by tudor on 3/10/2026.
//

#include "dispozitiv.h"

#include <string.h>

static const char* EMPTY_TEXT = "";

static void copy_text_field(char* dest, size_t capacity, const char* source)
{
    if (source == NULL)
    {
        dest[0] = '\0';
        return;
    }

    strncpy(dest, source, capacity - 1);
    dest[capacity - 1] = '\0';
}

int get_id(const Dispozitiv * d)
{
    if (d == NULL)
    {
        return -1;
    }
    return d->ID;
}

int get_quant(const Dispozitiv * d)
{
    if (d == NULL)
    {
        return -1;
    }
    return d->quant;
}
float get_price(const Dispozitiv * d)
{
    if (d == NULL)
    {
        return -1.0f;
    }
    return d->price;
}

const char* get_model(const Dispozitiv * d)
{
    if (d == NULL)
    {
        return EMPTY_TEXT;
    }
    return d->model;
}

const char* get_prod(const Dispozitiv * d)
{
    if (d == NULL)
    {
        return EMPTY_TEXT;
    }
    return d->prod;
}

const char* get_type(const Dispozitiv * d)
{
    if (d == NULL)
    {
        return EMPTY_TEXT;
    }
    return d->type;
}

Dispozitiv create_dispozitiv(int id, const char* type, const char* prod, const char* model, float price, int quant)
{
    Dispozitiv d;
    d.ID = id;
    copy_text_field(d.type, sizeof(d.type), type);
    copy_text_field(d.model, sizeof(d.model), model);
    copy_text_field(d.prod, sizeof(d.prod), prod);
    d.price = price;
    d.quant = quant;
    return d;
}

void set_price(Dispozitiv* d, float price)
{
    if (d == NULL)
    {
        return;
    }
    d->price = price;
}

void set_quant(Dispozitiv* d, int quant)
{
    if (d == NULL)
    {
        return;
    }
    d->quant = quant;
}
