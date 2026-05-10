#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

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

    while (fgets(line, sizeof(line), processed))
    {
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
    FILE *logs;

    char line[MAX_LINE];

    orders = fopen("orders.txt", "r");

    if (orders == NULL)
    {
        printf("Erro ao abrir orders.txt");
        return;
    }

    processed = fopen("processed.txt", "a");
    logs = fopen("logs/system.log", "a");

    if(processed == NULL || logs == NULL){
        printf("Erro ao abrir arquivos de sistema.\n");

        return;
    }

    fprintf(logs, "Iniciando processamento... \n");

    while (fgets(line, sizeof(line), orders))
    {
        line[strcspn(line, "\n")] = 0;

        if(is_order_processed(line)){
            printf("Pedido duplicado ignorado: %s\n", line);

            fprintf(logs, "Pedido duplicado ignorado: %s\n", line);

            continue;
        }

        sleep(2);

        if (rand() % 10 == 0)
        {

            fprintf(logs, "ERRO CRITICO: Falha inesperada.\n");

            printf("ERRO: Sistema legado falhou.\n");

            exit(1);
        }

        printf("Processando pedido: %s\n", line);

        fprintf(processed, "PROCESSADO: %s\n", line);

        fprintf(logs, "Pedido Processado: %s\n", line);
    }

    fprintf(logs, "Finalizado.\n");

    fclose(orders);
    fclose(processed);
    fclose(logs);

    printf("Processamento concluído.\n");
}

int main()
{

    srand(time(NULL));

    int option;

    printf("=== LEGACY ORDER SYSTEM ===\n");
    printf("1 - Processar Pedidos\n");
    printf("2 - Sair\n");
    printf("Escolha: ");

    scanf("%d", &option);

    switch (option)
    {
    case 1:
        process_orders();
        break;
    case 2:
        printf("Saindo...\n");
        break;

    default:
        printf("Opção inválida.\n");
        break;
    }

    return 0;
}