//
// Created by tudor on 3/10/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include "ui.h"

static void clear_input_line(void)
{
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

static int read_int_prompt(const char* prompt, int* value)
{
    printf("%s", prompt);
    if (scanf("%d", value) != 1)
    {
        clear_input_line();
        return 0;
    }

    clear_input_line();
    return 1;
}

static int read_float_prompt(const char* prompt, float* value)
{
    printf("%s", prompt);
    if (scanf("%f", value) != 1)
    {
        clear_input_line();
        return 0;
    }

    clear_input_line();
    return 1;
}

static int read_word_prompt(const char* prompt, char value[50])
{
    printf("%s", prompt);
    if (scanf("%49s", value) != 1)
    {
        clear_input_line();
        return 0;
    }

    clear_input_line();
    return 1;
}

static void print_menu()
{
    printf("\n===== MAGAZIN ELECTRONICE =====\n");
    printf("1. Adauga dispozitiv\n");
    printf("2. Modifica dispozitiv\n");
    printf("3. Sterge dispozitiv\n");
    printf("4. Afiseaza toate dispozitivele\n");
    printf("5. Vizualizare ordonata (pret/cantitate)\n");
    printf("6. Vizualizare filtrata\n");
    printf("0. Iesire\n");
    printf("Optiunea ta: ");
}

static void ui_add(UI* ui)
{
    int id, quant;
    float price;
    char type[50], prod[50], model[50];

    if (!read_int_prompt("ID: ", &id) ||
        !read_word_prompt("Tip: ", type) ||
        !read_word_prompt("Producator: ", prod) ||
        !read_word_prompt("Model: ", model) ||
        !read_float_prompt("Pret: ", &price) ||
        !read_int_prompt("Cantitate: ", &quant))
    {
        printf("Input invalid.\n");
        return;
    }

    int result = service_add(ui->service, id, type, prod, model, price, quant);

    if (result == 1)
    {
        printf("Operatie realizata cu succes.\n");
    }
    else if (result == 0)
    {
        printf("Date invalide.\n");
    }
    else
    {
        printf("Repo-ul este plin.\n");
    }
}

static void ui_update(UI* ui)
{
    int id, new_quant;
    float new_price;

    if (!read_int_prompt("ID dispozitiv de modificat: ", &id) ||
        !read_float_prompt("Pret nou: ", &new_price) ||
        !read_int_prompt("Cantitate noua: ", &new_quant))
    {
        printf("Input invalid.\n");
        return;
    }

    int result = service_update(ui->service, id, new_price, new_quant);

    if (result == 1)
    {
        printf("Dispozitiv modificat cu succes.\n");
    }
    else if (result == 0)
    {
        printf("Nu exista dispozitiv cu acest ID.\n");
    }
    else
    {
        printf("Date invalide.\n");
    }
}

static void ui_delete(UI* ui)
{
    int id;

    if (!read_int_prompt("ID dispozitiv de sters: ", &id))
    {
        printf("Input invalid.\n");
        return;
    }

    int result = service_delete(ui->service, id);

    if (result == 1)
    {
        printf("Dispozitiv sters cu succes.\n");
    }
    else
    {
        printf("Nu exista dispozitiv cu acest ID.\n");
    }
}

static void ui_print_all(UI* ui)
{
    int n = service_size(ui->service);

    if (n == 0)
    {
        printf("Nu exista dispozitive in stoc.\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        printf("ID: %d | Tip: %s | Producator: %s | Model: %s | Pret: %.2f | Cantitate: %d\n",
               service_get_id(ui->service, i),
               service_get_type(ui->service, i),
               service_get_prod(ui->service, i),
               service_get_model(ui->service, i),
               service_get_price(ui->service, i),
               service_get_quant(ui->service, i));
    }
}

static void print_device(const Dispozitiv* d)
{
    printf("ID: %d | Tip: %s | Producator: %s | Model: %s | Pret: %.2f | Cantitate: %d\n",
           get_id(d),
           get_type(d),
           get_prod(d),
           get_model(d),
           get_price(d),
           get_quant(d));
}

static void print_device_list(const Dispozitiv* list, int count)
{
    if (count == 0)
    {
        printf("Nu exista dispozitive care respecta criteriul.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        print_device(&list[i]);
    }
}

static int read_cmp_from_user(void)
{
    int op;

    printf("Alege comparatia: 1.<  2.=  3.>\n");
    if (!read_int_prompt("Optiunea ta: ", &op))
    {
        return 99;
    }

    if (op == 1)
    {
        return SERVICE_CMP_LESS;
    }
    if (op == 2)
    {
        return SERVICE_CMP_EQUAL;
    }
    if (op == 3)
    {
        return SERVICE_CMP_GREATER;
    }
    return 99;
}

static void ui_print_sorted(UI* ui)
{
    const int n = service_size(ui->service);
    if (n == 0)
    {
        printf("Nu exista dispozitive in stoc.\n");
        return;
    }

    int criterion, order;
    printf("Ordonare dupa: 1.Pret  2.Cantitate\n");
    if (!read_int_prompt("Optiunea ta: ", &criterion))
    {
        printf("Input invalid.\n");
        return;
    }

    printf("Sens: 1.Crescator  2.Descrescator\n");
    if (!read_int_prompt("Optiunea ta: ", &order))
    {
        printf("Input invalid.\n");
        return;
    }

    int key;
    if (criterion == 1)
    {
        key = SERVICE_SORT_KEY_PRICE;
    }
    else if (criterion == 2)
    {
        key = SERVICE_SORT_KEY_QUANTITY;
    }
    else
    {
        printf("Criteriu invalid.\n");
        return;
    }

    if (order != 1 && order != 2)
    {
        printf("Sens invalid.\n");
        return;
    }

    Dispozitiv* buffer = (Dispozitiv*)malloc((size_t)n * sizeof(Dispozitiv));
    if (buffer == NULL)
    {
        printf("Memorie insuficienta.\n");
        return;
    }

    int order_value = (order == 1) ? SERVICE_SORT_ORDER_ASC : SERVICE_SORT_ORDER_DESC;
    int count = service_sort(ui->service, buffer, n, key, order_value);
    if (count < 0)
    {
        printf("Parametri invalizi pentru ordonare.\n");
        free(buffer);
        return;
    }

    print_device_list(buffer, count);
    free(buffer);
}

static void ui_print_filtered(UI* ui)
{
    const int n = service_size(ui->service);
    if (n == 0)
    {
        printf("Nu exista dispozitive in stoc.\n");
        return;
    }

    int criterion;
    printf("Filtrare dupa: 1.Producator  2.Pret  3.Cantitate\n");
    if (!read_int_prompt("Optiunea ta: ", &criterion))
    {
        printf("Input invalid.\n");
        return;
    }

    Dispozitiv* buffer = (Dispozitiv*)malloc((size_t)n * sizeof(Dispozitiv));
    if (buffer == NULL)
    {
        printf("Memorie insuficienta.\n");
        return;
    }

    int count = -1;

    if (criterion == 1)
    {
        char producer[50];
        if (!read_word_prompt("Producator: ", producer))
        {
            printf("Input invalid.\n");
            free(buffer);
            return;
        }
        count = service_filter_by_producer(ui->service, producer, buffer, n);
    }
    else if (criterion == 2)
    {
        float value;
        if (!read_float_prompt("Pret de comparat: ", &value))
        {
            printf("Input invalid.\n");
            free(buffer);
            return;
        }

        int cmp = read_cmp_from_user();
        count = service_filter_by_price(ui->service, value, cmp, buffer, n);
    }
    else if (criterion == 3)
    {
        int value;
        if (!read_int_prompt("Cantitate de comparat: ", &value))
        {
            printf("Input invalid.\n");
            free(buffer);
            return;
        }

        int cmp = read_cmp_from_user();
        count = service_filter_by_quantity(ui->service, value, cmp, buffer, n);
    }
    else
    {
        printf("Criteriu invalid.\n");
        free(buffer);
        return;
    }

    if (count < 0)
    {
        printf("Parametri invalizi pentru filtrare.\n");
        free(buffer);
        return;
    }

    print_device_list(buffer, count);
    free(buffer);
}

void ui_init(UI* ui, Service* s)
{
    if (ui == NULL)
    {
        return;
    }

    ui->service = s;
}

void ui_run(UI* ui)
{
    int cmd;
    if (ui == NULL || ui->service == NULL)
    {
        printf("UI/Service neinitializat.\n");
        return;
    }

    while (1)
    {
        print_menu();
        if (!read_int_prompt("", &cmd))
        {
            printf("Input invalid.\n");
            continue;
        }

        switch (cmd)
        {
            case 1:
                ui_add(ui);
                break;
            case 2:
                ui_update(ui);
                break;
            case 3:
                ui_delete(ui);
                break;
            case 4:
                ui_print_all(ui);
                break;
            case 5:
                ui_print_sorted(ui);
                break;
            case 6:
                ui_print_filtered(ui);
                break;
            case 0:
                printf("La revedere.\n");
                return;
            default:
                printf("Optiune invalida.\n");
        }
    }
}
