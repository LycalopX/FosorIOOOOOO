#include "constants.h"

void fechar_conta(conta *vet, int *size, transaction *transacs, int *ntransacs)
{
    char linha[3][30];
    for (int i = 0; i < 3; i++)
    {
        fgets(linha[i], sizeof(linha[i]), stdin);
        linha[i][strcspn(linha[i], "\n")] = 0;
    }

    int a = 0;
    for (int i = 0; i < 7; i++)
    {
        a += linha[0][i] - '0';
    }
    if ((a % 10) != (linha[0][7] - '0'))
    {
        printf("ERROCONTA\n");
        return;
    }

    a = 0;
    int b = 10 * (linha[1][9] - '0') + (linha[1][10] - '0');
    for (int i = 0; i < 9; i++)
    {
        a += linha[1][i] - '0';
    }
    if (a != b)
    {
        printf("ERROCPF\n");
        return;
    }
    int i;
    for(i = 0; i < *size; i++)
    {
        if (vet[i].nro_conta == atoi(linha[0])) break;
    }
    if (i == *size)
    {
        printf("ERROINEXISTENTE\n");
        return;
    }
    if(vet[i].saldo < 0){
        transacs[*ntransacs].nro_conta = vet[i].nro_conta;
        transacs[*ntransacs].tipo = 'P';
        transacs[*ntransacs].valor = -vet[i].saldo;
        (*ntransacs)++;
    }
    else if(vet[i].saldo > 0){
        transacs[*ntransacs].nro_conta = vet[i].nro_conta;
        transacs[*ntransacs].tipo = 'S';
        transacs[*ntransacs].valor = vet[i].saldo;
        (*ntransacs)++;
    }
    vet[i].nro_conta = -1; // Marca a conta como fechada
    transacs[*ntransacs].nro_conta = vet[i].nro_conta;
    transacs[*ntransacs].tipo = 'F';
    transacs[*ntransacs].valor = 0.0;
    (*ntransacs)++;
    return;
}