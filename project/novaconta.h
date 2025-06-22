#include "constants.h"

void abrir_conta(conta *vet, int *size, transaction *transacs, int *ntransacs)
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

    for(int i = 0; i < *size; i++)
    {
        if (vet[i].nro_conta == atoi(linha[0]))
        {
            printf("ERRODUPLICADA\n");
            return;
        }
    }
    *size += 1;
    vet = (conta*)realloc(vet, (*size) * sizeof(conta));
    if (vet == NULL)
    {
        printf("ERROALOCACAO\n");
        return;
    }
    vet[*size - 1].nro_conta = atoi(linha[0]);
    vet[*size - 1].cpf = atoll(linha[1]);
    strncpy(vet[*size - 1].nome, linha[2], sizeof(vet[*size - 1].nome) - 1);
    vet[*size - 1].nome[sizeof(vet[*size - 1].nome) - 1] = '\0';
    vet[*size - 1].saldo = 0.0;

    transacs[*ntransacs].nro_conta = vet[*size - 1].nro_conta;
    transacs[*ntransacs].tipo = 'A';
    transacs[*ntransacs].valor = 0.0;
    (*ntransacs)++;

    printf("OKCONTA\n");
    return;
}