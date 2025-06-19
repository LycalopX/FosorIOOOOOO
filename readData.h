#include "constants.h"

FILE *arq;

void read_data(conta **vet, int *size, transferencia **transf, int *ntransf)
{
    arq = fopen("contasin.csv", "r");
    if (arq == NULL)
    {
        return; // Erro ao abrir o arquivo
    }

    char linha[100];
    fgets(linha, sizeof(linha), arq); // Lê a primeira linha com o número de contas
    *size = atoi(linha);

    *vet = (conta *)calloc(*size, sizeof(conta));
    if (*vet == NULL)
    {
        fclose(arq);
        return; // Erro ao alocar memória
    }

    int i = 0;
    while (fgets(linha, sizeof(linha), arq))
    {
        linha[strcspn(linha, "\n")] = 0;
        char *tok = strtok(linha, ",");
        (*vet)[i].nro_conta = atoi(tok);

        tok = strtok(NULL, ",");
        (*vet)[i].cpf = atoll(tok);

        tok = strtok(NULL, ",");
        strncpy((*vet)[i].nome, tok, sizeof((*vet)[i].nome) - 1);
        (*vet)[i].nome[sizeof((*vet)[i].nome) - 1] = '\0';

        tok = strtok(NULL, ",");
        (*vet)[i].saldo = atof(tok);
        i++;
    }

    fclose(arq);
    return;
}

void exibe_dados(int size, conta *vet)
{
    if (vet == NULL)
    {
        printf("Error: vet is NULL.\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        printf("%d,%lld,%s,%.2lf\n", vet[i].nro_conta, vet[i].cpf, vet[i].nome, vet[i].saldo);
    }
}
