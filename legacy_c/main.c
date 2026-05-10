#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "processor.h"

int main(int argc, char *argv[])
{

    srand(time(NULL));

    if(argc > 1 && strcmp(argv[1], "auto") == 0){
        return process_orders();
    }

    int option;

    printf("=== LEGACY ORDER SYSTEM ===\n");
    printf("1 - Processar pedidos\n");
    printf("2 - Sair\n");
    printf("Escolha: ");

    scanf("%d", &option);
   

    switch (option)
    {
    case 1:
        return process_orders();

    case 2:
        printf("Saindo...\n");
        return 0;

    default:
        printf("Opção inválida.\n");
        return 1;
    }
}