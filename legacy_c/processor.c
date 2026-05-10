#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "processor.h"
#include "logger.h"

#define MAX_LINE 256

int is_order_processed(char *order_id)
{

    FILE *processed;

    char line[MAX_LINE];

    processed = fopen("processed.txt", "r");

    if (processed == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), processed) != NULL)
    {
        line[strcspn(line, "\n")] = 0;

        if (strstr(line, order_id) != NULL)
        {
            fclose(processed);

            return 1;
        }
    }

    fclose(processed);

    return 0;
}

void process_orders()
{
    FILE *orders;
    FILE *processed;

    char line[MAX_LINE];

    orders = fopen("orders.txt", "r");

    if (orders == NULL)
    {
        printf("Erro ao abrir orders.txt\n");

        write_log("ERRO: Falha ao abrir orders.txt");

        return;
    }

    processed = fopen("processed.txt", "a");

    if (processed == NULL)
    {

        printf("Erro ao abrir processed.txt\n");

        write_log("ERRO: Falha ao abrir processed.txt");

        return;
    }

    write_log("Iniciando processamento");

    while (fgets(line, sizeof(line), orders))
    {
        line[strcspn(line, "\n")] = 0;

        if(is_order_processed(line)){
            printf("Pedido duplicado ignorado: %s\n", line);

            write_log("Pedido duplicado ignorado");

            continue;
        }

        sleep(2);

        if(rand() % 10 == 0) {
            write_log("ERRO CRÍTICO: Falha inesperada");

            printf("ERRO: Sistema legado falhou.\n");

            exit(1);
        }

        printf("Processando pedido: %s\n", line);

        fprintf(processed, "PROCESSADO: %s\n", line);

        write_log("Pedido processado");
    }

    write_log("Finalizado");

    fclose(orders);
    fclose(processed);

    printf("Processamento concluído.\n");
}