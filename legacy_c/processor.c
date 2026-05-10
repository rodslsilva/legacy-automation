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
        // remove newline
        line[strcspn(line, "\n")] = 0;

        // comparação EXATA (corrigido)
        if (strcmp(line, order_id) == 0)
        {
            fclose(processed);
            return 1;
        }
    }

    fclose(processed);
    return 0;
}


int process_orders()
{
    FILE *orders;
    FILE *processed;
    char line[MAX_LINE];

    orders = fopen("orders.txt", "r");

    if (orders == NULL)
    {
        printf("Erro ao abrir orders.txt\n");
        write_log("ERRO: Falha ao abrir orders.txt");
        return 1;
    }

    processed = fopen("processed.txt", "a");

    if (processed == NULL)
    {
        printf("Erro ao abrir processed.txt\n");
        write_log("ERRO: Falha ao abrir processed.txt");

        fclose(orders);
        return 1;
    }

    write_log("Iniciando processamento");

    while (fgets(line, sizeof(line), orders))
    {
        line[strcspn(line, "\n")] = 0;

        if (is_order_processed(line))
        {
            printf("Pedido duplicado ignorado: %s\n", line);
            write_log("Pedido duplicado ignorado");
            continue;
        }

        sleep(1);

        // simulação de falha
        if (rand() % 10 == 0)
        {
            write_log("ERRO CRÍTICO: Falha inesperada");

            printf("ERRO: Sistema legado falhou.\n");

            fclose(orders);
            fclose(processed);

            return 1;
        }

        printf("Processando pedido: %s\n", line);

        fprintf(processed, "PROCESSADO: %s\n", line);

        write_log("Pedido processado");
    }

    write_log("Finalizado");

    fclose(orders);
    fclose(processed);

    printf("Processamento concluído.\n");

    return 0;
}