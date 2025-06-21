#include "constants.h"

void ver_saldo(conta *vet, int size, int tipo)
{
    char linha[30];
    fgets(linha, sizeof(linha), stdin);
    linha[strcspn(linha, "\n")] = 0;
    if (tipo == 0)
    {
        int a = 0;
        for (int i = 0; i < 7; i++)
        {
            a += linha[i] - '0';
        }
        if ((a % 10) != (linha[7] - '0'))
        {
            printf("ERROCONTA\n");
            return;
        }
        int nro_conta = atoi(linha);
        for (int i = 0; i < size; i++)
        {
            if (vet[i].nro_conta == nro_conta)
            {
                printf("SALDO %.2f\n", vet[i].saldo);
                return;
            }
        }
    }
    else if (tipo==1)
    {
        int a = 0;
        int b = 10 * (linha[9] - '0') + (linha[10] - '0');
        for (int i = 0; i < 9; i++)
        {
            a += linha[i] - '0';
        }
        if (a != b)
        {
            printf("ERROCPF\n");
            return;
        }
        long long int cpf = atoll(linha);
        for (int i = 0; i < size; i++)
        {
            if (vet[i].cpf == cpf)
            {
                printf("CONTA %08d - SALDO %.2f\n", vet[i].nro_conta, vet[i].saldo);
                return;
            }
        }
    }
    printf("ERROINEXISTENTE\n");
    return;
}