#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 256

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

    fprintf(logs, "Iniciando processamento... \n");

    while (fgets(line, sizeof(line), orders))
    {
        line[strcspn(line, "\n")] = 0;

        sleep(2);

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