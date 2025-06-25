// Definições de constantes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct conta
{
    int nro_conta;
    long long int cpf;
    char nome[30];
    double saldo;
} conta;

typedef struct transaction
{
    int nro_conta;
    char tipo;
    double valor;
} transaction;

/**
 * A função abre o arquivo "contasin.csv", aloca dinamicamente memória para armazenar
 * todas as contas e as carrega. Durante o carregamento, para cada conta com saldo
 * negativo, é aplicado um juro de 1% (aumentando o valor devido). Cada aplicação de
 * juros é registrada como uma nova transação no vetor 'transf'.
 * A função exibe "OK" na tela se a leitura for bem-sucedida ou "ERRO" em caso de falha
 */
void read_data(conta **vet, int *size, transaction *transacs, int *ntransacs)
{
    FILE *arq;
    arq = fopen("contasin.csv", "r");
    if (arq == NULL)
    {
        return;
    }

    char linha[100];
    fgets(linha, sizeof(linha), arq); // Lê a primeira linha com o número de contas
    *size = atoi(linha);

    *vet = (conta *)calloc(*size, sizeof(conta));
    if (*vet == NULL)
    {
        fclose(arq);
        return;
    }

    int i = 0;

    while (fgets(linha, sizeof(linha), arq))
    {
        // Pega os 4 dados
        sscanf(linha, "%d, %lld, %[^,], %lf",
               &(*vet)[i].nro_conta,
               &(*vet)[i].cpf,
               (*vet)[i].nome,
               &(*vet)[i].saldo);

        if ((*vet)[i].saldo < 0)
        {
            double a = (*vet)[i].saldo * 0.01;
            (*vet)[i].saldo += a; // Aplica o juro de 1%

            // Registra a transação de juros
            transacs[*ntransacs].nro_conta = (*vet)[i].nro_conta;
            transacs[*ntransacs].tipo = 'J';
            transacs[*ntransacs].valor = a;
            (*ntransacs)++;
        }
        i++;
    }

    fclose(arq);
    return;
}

//Cria uma nova conta bancária após validar os dados de entrada.
void abrir_conta(conta **vet, int *size, transaction *transacs, int *ntransacs)
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
    if ((a % 100) != b)
    {
        printf("ERROCPF\n");
        return;
    }

    for (int i = 0; i < *size; i++)
    {
        if ((*vet)[i].nro_conta == atoi(linha[0]))
        {
            printf("ERRODUPLICADA\n");
            return;
        }
    }
    conta *temp_ptr = (conta *)realloc(*vet, (*size + 1) * sizeof(conta));
    if (temp_ptr == NULL)
    {
        printf("ERROALOCACAO\n");
        return;
    }
    *vet = temp_ptr;
    *size += 1;

    (*vet)[*size - 1].nro_conta = atoi(linha[0]);
    (*vet)[*size - 1].cpf = atoll(linha[1]);
    strncpy((*vet)[*size - 1].nome, linha[2], sizeof((*vet)[*size - 1].nome) - 1);
    (*vet)[*size - 1].nome[sizeof((*vet)[*size - 1].nome) - 1] = '\0';
    (*vet)[*size - 1].saldo = 0.0;

    transacs[*ntransacs].nro_conta = (*vet)[*size - 1].nro_conta;
    transacs[*ntransacs].tipo = 'A';
    transacs[*ntransacs].valor = 0.0;
    (*ntransacs)++;

    printf("OKCONTA\n");
    return;
}

/**
 * Fecha uma conta bancária existente após validação.
 * Se a conta tiver saldo, registra uma transação de depósito ou pagamento para zerá-lo. 
 * Marca a conta como fechada e registra uma transação 'F' (Fechamento).
 */
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
    if ((a % 100) != b)
    {
        printf("ERROCPF\n");
        return;
    }
    int i;
    for (i = 0; i < *size; i++)
    {
        if (vet[i].nro_conta == atoi(linha[0]))
            break;
    }
    if (i == *size)
    {
        printf("ERROINEXISTENTE\n");
        return;
    }
    if (vet[i].saldo < 0)
    {
        transacs[*ntransacs].nro_conta = vet[i].nro_conta;
        transacs[*ntransacs].tipo = 'D';
        transacs[*ntransacs].valor = -vet[i].saldo;
        (*ntransacs)++;
        printf("PAGAR %.2f\n", -vet[i].saldo);
    }
    else if (vet[i].saldo > 0)
    {
        transacs[*ntransacs].nro_conta = vet[i].nro_conta;
        transacs[*ntransacs].tipo = 'S';
        transacs[*ntransacs].valor = vet[i].saldo;
        (*ntransacs)++;
        printf("SAQUE %.2f\n", vet[i].saldo);
    }
    else if (vet[i].saldo == 0)
    {
        printf("NADA A PAGAR OU SACAR\n");
    }
    transacs[*ntransacs].nro_conta = vet[i].nro_conta;
    transacs[*ntransacs].tipo = 'F';
    transacs[*ntransacs].valor = 0.0;
    (*ntransacs)++;
    vet[i].nro_conta = -1; // Marca a conta como fechada
    return;
}

/**
 * Exibe o saldo de uma conta, buscando por número da conta ou por CPF.
 * O tipo indica se a busca é por número da conta (0) ou por CPF (1).
 */
void ver_saldo(conta *vet, int size, int tipo)
{
    int cont = 0;
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
                cont++;
            }
        }
    }
    else if (tipo == 1)
    {
        int a = 0;
        int b = 10 * (linha[9] - '0') + (linha[10] - '0');
        for (int i = 0; i < 9; i++)
        {
            a += linha[i] - '0';
        }
        if ((a % 100) != b)
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
                cont++;
            }
        }
    }
    if (cont == 0)
    {
        printf("ERROINEXISTENTE\n");
    }
    return;
}

/**
 * Verifica se a conta existe e retorna seu índice.
 * SOD é uma string usada para dar contexto às mensagens de erro.
 */
int contaExiste(int nro_conta, conta *vet, int size, char *SOD)
{
    int n = nro_conta, sum = 0, rest = 0;

    do
    {
        n = n / 10;

        rest = n % 10;
        sum += rest;

    } while (n != rest);

    if ((nro_conta % 10) != (sum % 10))
    {

        printf("ERROCONTA%s\n", SOD);
        return -1;
    };

    for (int i = 0; i < size; i++)
    {
        if (vet[i].nro_conta == nro_conta)
        {
            return i;
        }
    }

    printf("ERROINEXISTENTE%s\n", SOD);
    return -1;
}

//Realiza um depósito de uma conta corrente após validar os dados de entrada.
void realizar_deposito(int posicaoConta, double valor, conta *contas, int nro_conta, transaction *transacs, int *ntransacs)
{
    contas[posicaoConta].saldo += valor;

    transacs[*ntransacs].nro_conta = nro_conta;
    transacs[*ntransacs].tipo = 'D';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    printf("CONTA %08d - DEP %.2f\n", nro_conta, valor);
}


//Realiza um saque de uma conta corrente após validar os dados de entrada.
void realizar_saque(int posicaoConta, double valor, conta *contas, int nro_conta, transaction *transacs, int *ntransacs)
{
    contas[posicaoConta].saldo -= valor;

    transacs[*ntransacs].nro_conta = nro_conta;
    transacs[*ntransacs].tipo = 'S';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    printf("CONTA %08d - SAQUE %.2f\n", nro_conta, valor);
};

//Efetua um pagamento de uma conta corrente após validar os dados de entrada.
void realizar_pagamento(int posicaoConta, double valor, conta *contas, int nro_conta, transaction *transacs, int *ntransacs)
{
    contas[posicaoConta].saldo -= valor;

    transacs[*ntransacs].nro_conta = nro_conta;
    transacs[*ntransacs].tipo = 'P';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    printf("CONTA %08d - PGTO %.2f\n", nro_conta, valor);
};

//Efetua uma transferência de valor entre duas contas correntes.
void realizar_transferencia(int posicaoOrigem, int posicaoDestino, double valor, conta *contas, int nro_origem, int nro_destino, transaction *transacs, int *ntransacs)
{

    contas[posicaoOrigem].saldo -= valor;
    contas[posicaoDestino].saldo += valor;

    transacs[*ntransacs].nro_conta = nro_origem;
    transacs[*ntransacs].tipo = 'S';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    transacs[*ntransacs].nro_conta = nro_destino;
    transacs[*ntransacs].tipo = 'D';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    printf("DA CONTA %08d PARA CONTA %08d - TRANSF %.2lf\n", nro_origem, nro_destino, valor);
};

//Salva os dados das contas e transações em arquivos CSV.
//Verifica se a conta está fechada (nro_conta < 0) e não a inclui no arquivo de saída.
//O arquivo "contasout.csv" contém os dados das contas e "operaout.csv" contém as transações.
int salvar_dados_em_disco(conta *contas, int size, transaction *transacs, int ntransacs)
{
    FILE *file = fopen("contasout.csv", "w");

    if (file == NULL)
    {
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (contas[i].nro_conta < 0) {
            (size)--;
        }
    }

    fprintf(file, "%d, %d\n", size, 4);

    for (int i = 0; i < size; i++)
    {
        if (contas[i].nro_conta < 0)
        {
            continue; // Pula contas fechadas
        }
        fprintf(file, "%08d, %lld, %s, %.2lf\n",
                contas[i].nro_conta,
                contas[i].cpf,
                contas[i].nome,
                contas[i].saldo);
    }

    fclose(file);

    file = fopen("operaout.csv", "w");

    if (file == NULL)
    {
        return 0;
    }

    for (int i = 0; i < ntransacs; i++)
    {
        fprintf(file, "%08d, %c, %.2lf\n",
                transacs[i].nro_conta,
                transacs[i].tipo,
                transacs[i].valor);
    }

    fclose(file);
    file = NULL;

    return 1;
};

int main(int argc, char *argv[])
{
    printf("EXEC MAIN\n");

    int n = -2, size = 0, ntransacs = 0, loop = 1;
    conta *vet = calloc(1, sizeof(conta));
    transaction *transacs = calloc(1000, sizeof(transaction));

    while (loop)
    {
        if (ntransacs >= 1000)
        {
            printf("NUMERO DE TRANSACOES MAXIMAS DO DIA ALCANCADAS\nEXEC FIM\n");
            free(vet);
            free(transacs);
            vet = NULL;
            transacs = NULL;
            return 0;
        }

        scanf("%d", &n);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (n)
        {

        // Se n for -1, encerra o programa
        case -1:
        {
            loop = 0;
            break;
        }

        // 0 – Carrega dados do arquivo “contasin.csv” (e atualiza com cobrança de juros em contas negativas)
        case 0:
        {

            read_data(&vet, &size, transacs, &ntransacs);
            if (vet == NULL || size <= 0)
            {
                printf("ERRO\n");
            }
            else
            {
                printf("OK\n");
            }
            break;
        }

        // 1 – Abre nova conta corrente
        case 1:
        {
            abrir_conta(&vet, &size, transacs, &ntransacs);
            break;
        }

        // 2 – Fecha conta corrente existente
        case 2:
        {
            fechar_conta(vet, &size, transacs, &ntransacs);
            break;
        }

        // 3 – Consulta o saldo de conta corrente (pelo nro. da conta)
        case 3:
        {
            ver_saldo(vet, size, 0);
            break;
        }

        // 4 – Consulta o saldo de conta corrente (pelo nro. do CPF)
        case 4:
        {
            ver_saldo(vet, size, 1);
            break;
        }

        // 5 – Realiza o depósito de um valor em uma conta
        case 5:
        {
            int nro_conta;
            double valor;
            scanf("%d %lf", &nro_conta, &valor);

            int posicao = contaExiste(nro_conta, vet, size, "");
            if (posicao >= 0)
            {
                realizar_deposito(posicao, valor, vet, nro_conta, transacs, &ntransacs);
            }

            break;
        }

        // 6 – Realiza o saque de um valor de uma conta
        case 6:
        {
            int nro_conta;
            double valor;
            scanf("%d %lf", &nro_conta, &valor);

            int posicao = contaExiste(nro_conta, vet, size, "");
            if (posicao >= 0)
            {
                realizar_saque(posicao, valor, vet, nro_conta, transacs, &ntransacs);
            }
            break;
        }

        // 7 – Realiza um pagamento com o saldo de uma conta
        case 7:
        {
            int nro_conta;
            double valor;
            scanf("%d %lf", &nro_conta, &valor);

            int posicao = contaExiste(nro_conta, vet, size, "");
            if (posicao >= 0)
            {
                realizar_pagamento(posicao, valor, vet, nro_conta, transacs, &ntransacs);
            }
            break;
        }

        // 8 – Transfere valor de uma conta para outra conta
        case 8:
        {
            int nro_conta_origem, nro_conta_destino;
            double valor;
            scanf("%d %d %lf", &nro_conta_origem, &nro_conta_destino, &valor);

            int posicao_origem = contaExiste(nro_conta_origem, vet, size, "(O)");
            int posicao_destino = contaExiste(nro_conta_destino, vet, size, "(D)");

            if (posicao_origem >= 0 && posicao_destino >= 0)
            {
                realizar_transferencia(posicao_origem, posicao_destino, valor, vet, nro_conta_origem, nro_conta_destino, transacs, &ntransacs);
            }

            break;
        }

        // 9 – Salva em disco a posição atual do dia (saldos
        case 9:
        {
            if (salvar_dados_em_disco(vet, size, transacs, ntransacs))
            {
                printf("OK\n");
            }
            else
            {
                printf("ERRO\n");
            }
            break;
        }

        default:
            printf("EXEC INVALIDO\n");
            break;
        }
    }

    printf("EXEC FIM\n");
    free(vet);
    free(transacs);
    vet = NULL;
    transacs = NULL;

    return 0;
}