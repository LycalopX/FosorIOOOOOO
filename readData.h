#include "constants.h"

/**
 * @brief Lê os dados de contas do arquivo "contasin.csv" e processa juros em saldos negativos.
 * @param vet Ponteiro para o vetor de contas que será alocado e preenchido.
 * @param size Ponteiro para a variável que armazenará o número de contas lidas.
 * @param transacs Ponteiro para o vetor onde as transações de juros serão armazenadas.
 * @param ntransacs Ponteiro para a variável que armazena o número total de transações.
 *
 * A função abre o arquivo "contasin.csv", aloca dinamicamente memória para armazenar
 * todas as contas e as carrega. Durante o carregamento, para cada conta com saldo
 * negativo, é aplicado um juro de 1% (aumentando o valor devido). Cada aplicação de
 * juros é registrada como uma nova transação no vetor 'transf'.
 * A função exibe "OK" na tela se a leitura for bem-sucedida ou "ERRO" em caso de falha
 * (ex: não conseguir abrir o arquivo).
 */
void read_data(conta **vet, int *size, transaction **transacs, int *ntransacs)
{

    FILE *arq;

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
