#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "processor.h"

int main(){
    srand(time(NULL));

    int option;

    printf("=== LEGACY ORDER SYSTEM ===\n");
    printf("1 - Processar pedidos\n");
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