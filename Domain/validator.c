//
// Created by tudor on 3/10/2026.
//

#include "validator.h"
#include <string.h>

static int validate_text_field(const char* value, size_t max_capacity)
{
    if (value == NULL)
    {
        return 0;
    }

    size_t length = strlen(value);
    if (length == 0)
    {
        return 0;
    }

    return length < max_capacity;
}

int validate_dispozitiv(int id, const char* type, const char* prod, const char* model, float price, int quant)
{
    if (id <= 0)
    {
        return 0;
    }
    if (!validate_text_field(type, sizeof(((Dispozitiv*)0)->type)))
    {
        return 0;
    }
    if (!validate_text_field(prod, sizeof(((Dispozitiv*)0)->prod)))
    {
        return 0;
    }
    if (!validate_text_field(model, sizeof(((Dispozitiv*)0)->model)))
    {
        return 0;
    }
    if (price <= 0)
    {
        return 0;
    }
    if (quant < 0)
    {
        return 0;
    }
    return 1;
}

int validate_dispozitiv_obj(const Dispozitiv* d)
{
    if (d == NULL)
    {
        return 0;
    }

    return validate_dispozitiv(
        get_id(d),
        get_type(d),
        get_prod(d),
        get_model(d),
        get_price(d),
        get_quant(d)
    );
}
