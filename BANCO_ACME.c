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

/**
 * @brief Cria uma nova conta bancária após validar os dados de entrada.
 * @param vet Ponteiro para o array de contas. O array será realocado para acomodar a nova conta.
 * @param size Ponteiro para o inteiro que armazena o número atual de contas. Este valor é incrementado em caso de sucesso.
 * @param transacs Ponteiro para o array de transações.
 * @param ntransacs Ponteiro para o inteiro que armazena o número atual de transações. Este valor é incrementado em caso de sucesso.
 * @note Lê o número da conta, CPF e nome da entrada padrão (stdin). Valida os dígitos da conta e do CPF.
 * Imprime erros para dados inválidos (ERROCONTA, ERROCPF), contas duplicadas (ERRODUPLICADA),
 * ou falha na alocação de memória (ERROALOCACAO). Em caso de sucesso, adiciona uma transação 'A' (Abertura).
 */
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

    for (int i = 0; i < *size; i++)
    {
        if (vet[i].nro_conta == atoi(linha[0]))
        {
            printf("ERRODUPLICADA\n");
            return;
        }
    }
    *size += 1;
    vet = (conta *)realloc(vet, (*size) * sizeof(conta));
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

/**
 * @brief Fecha uma conta bancária existente após validação.
 * @param vet Ponteiro para o array de contas.
 * @param size Ponteiro para o inteiro que armazena o número atual de contas.
 * @param transacs Ponteiro para o array de transações.
 * @param ntransacs Ponteiro para o inteiro que armazena o número atual de transações. Este valor é incrementado.
 * @note Lê o número da conta e o CPF da entrada padrão (stdin). Se a conta tiver saldo, registra uma
 * transação de saque ('S') ou pagamento ('P') para zerá-lo. Marca a conta como fechada
 * e registra uma transação 'F' (Fechamento). Imprime erros para dados inválidos (ERROCONTA, ERROCPF)
 * ou se a conta não existir (ERROINEXISTENTE).
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
    if (a != b)
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
        transacs[*ntransacs].tipo = 'P';
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
    vet[i].nro_conta = -1; // Marca a conta como fechada
    transacs[*ntransacs].nro_conta = vet[i].nro_conta;
    transacs[*ntransacs].tipo = 'F';
    transacs[*ntransacs].valor = 0.0;
    (*ntransacs)++;
    return;
}

/**
 * @brief Exibe o saldo de uma conta, buscando por número da conta ou por CPF.
 * @param vet Ponteiro para o array de contas.
 * @param size O número atual de contas no array.
 * @param tipo Um inteiro que especifica o tipo de busca: 0 para número da conta, 1 para CPF.
 * @note Lê um número de conta ou um CPF da entrada padrão (stdin), com base no parâmetro `tipo`.
 * Valida a entrada e imprime o saldo correspondente.
 * Imprime erros para dados inválidos (ERROCONTA, ERROCPF) ou se o registro não for encontrado (ERROINEXISTENTE).
 */
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
    else if (tipo == 1)
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

/**
 * @brief Verifica se uma conta existe e retorna seu índice.
 * @param nro_conta O número da conta a ser procurado.
 * @param vet Ponteiro para o array de contas.
 * @param size O número atual de contas no array.
 * @param SOD Uma string ("ORIGEM", "DESTINO", etc.) para ser anexada às mensagens de erro para dar contexto.
 * @return O índice da conta no array, se encontrada; caso contrário, -1.
 * @note Imprime uma mensagem de erro (ERROCONTA ou ERROINEXISTENTE) sufixada com a string SOD se o
 * número da conta for inválido ou não for encontrado.
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

/**
 * @brief Realiza um depósito em uma conta corrente específica.
 * @param posicaoConta A posição na string da conta que receberá o depósito.
 * @param valor O valor a ser depositado.
 * @param contas Vetor que armazena todas as contas existentes.
 * @param nro_conta Número da conta que receberá o depósito
 * @param transacs Vetor para registrar a nova transação de depósito.
 * @param ntransacs Ponteiro para a variável que indica o número de transações totais.
 *
 * A função primeiro valida o número da conta. Se for inválido, exibe "ERROCONTA".
 * Em seguida, busca a conta no sistema. Se não a encontrar, exibe "ERROINEXISTENTE".
 * Se a conta for válida e existir, o valor do depósito é somado ao saldo atual
 * e a operação é registrada como uma nova transação. Em caso de sucesso, exibe uma
 * mensagem de confirmação no formato "CONTA [numero] - DEP [valor]".
 */
void realizar_deposito(int posicaoConta, double valor, conta *contas, int nro_conta, transaction *transacs, int *ntransacs)
{
    contas[posicaoConta].saldo += valor;

    transacs[*ntransacs].nro_conta = nro_conta;
    transacs[*ntransacs].tipo = 'D';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    printf("CONTA %08d - DEP %.2f\n", nro_conta, valor);
}

/**
 * @brief Realiza um saque de uma conta corrente específica.
 * @param posicaoConta A posição na string da conta que receberá o depósito.
 * @param valor O valor a ser depositado.
 * @param contas Vetor que armazena todas as contas existentes.
 * @param nro_conta Número da conta que receberá o depósito
 * @param transacs Vetor para registrar a nova transação de depósito.
 * @param ntransacs Ponteiro para a variável que indica o número de transações totais.
 *
 * A função valida o número da conta (retornando "ERROCONTA" se inválido) e verifica
 * sua existência (retornando "ERROINEXISTENTE" se não encontrada). Se a conta for
 * válida, o valor do saque é subtraído do saldo e a operação é registrada como uma
 * transação. Em caso de sucesso, exibe "CONTA [numero] - SAQUE [valor]".
 */
void realizar_saque(int posicaoConta, double valor, conta *contas, int nro_conta, transaction *transacs, int *ntransacs)
{
    contas[posicaoConta].saldo -= valor;

    transacs[*ntransacs].nro_conta = nro_conta;
    transacs[*ntransacs].tipo = 'S';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    printf("CONTA %08d - SAQUE %.2f\n", nro_conta, valor);
};

/**
 * @brief Efetua um pagamento a partir de uma conta corrente.
 * @param posicaoConta O índice da conta no vetor de contas.
 * @param valor O valor do pagamento a ser debitado.
 * @param contas Ponteiro para o vetor de todas as contas.
 * @param nro_conta O número da conta que está efetuando o pagamento.
 * @param transacs Ponteiro para o vetor de transações.
 * @param ntransacs Ponteiro para o contador total de transações.
 *
 * Esta função assume que a conta já foi validada e encontrada. Ela subtrai
 * o valor do saldo da conta, registra uma nova transação do tipo 'P' (Pagamento),
 * e exibe a confirmação no formato "CONTA [numero] - PGTO [valor]".
 */
void realizar_pagamento(int posicaoConta, double valor, conta *contas, int nro_conta, transaction *transacs, int *ntransacs)
{
    contas[posicaoConta].saldo -= valor;

    transacs[*ntransacs].nro_conta = nro_conta;
    transacs[*ntransacs].tipo = 'P';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    printf("CONTA %08d - PGTO %.2f\n", nro_conta, valor);
};

/**
 * @brief Efetua uma transferência de valor entre duas contas correntes.
 * @param posicaoOrigem O índice da conta de origem no vetor.
 * @param posicaoDestino O índice da conta de destino no vetor.
 * @param valor O valor a ser transferido.
 * @param contas Ponteiro para o vetor de todas as contas.
 * @param nro_origem Número da conta de origem (para log e impressão).
 * @param nro_destino Número da conta de destino (para log e impressão).
 * @param transacs Ponteiro para o vetor de transações.
 * @param ntransacs Ponteiro para o contador total de transações.
 *
 * Esta função assume que ambas as contas já foram validadas e encontradas.
 * Ela subtrai o valor do saldo da conta de origem, adiciona o mesmo valor ao saldo
 * da conta de destino e registra uma única transação do tipo 'T' (Transferência).
 * Ao final, exibe a confirmação no formato "DA CONTA [origem] PARA CONTA [destino] - TRANSF [valor]".
 */
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

/**
 * @brief Salva o estado atual das contas e transações em arquivos CSV.
 * @param contas Ponteiro para o vetor com os dados de todas as contas.
 * @param size O número total de contas a serem salvas.
 * @param transacs Ponteiro para o vetor com todas as transações realizadas.
 * @param ntransacs O número total de transações a serem salvas.
 *
 * Esta função cria ou sobrescreve dois arquivos de saída para persistir os dados da sessão:
 *
 * 1.  `contasout.csv`: Salva a situação atual das contas.
 * - O arquivo inicia com um cabeçalho no formato [N_CONTAS],[N_COLUNAS].
 * - Cada linha subsequente representa uma conta com seus dados (número, CPF, nome, saldo),
 * com o valor do saldo sempre formatado para ter duas casas decimais.
 *
 * 2.  `operaout.csv`: Salva o histórico de todas as transações (máximo de 1000).
 * - Cada linha representa uma transação com 3 campos: número da conta, tipo e valor.
 * - O valor da transação também é formatado com duas casas decimais.
 * - Os tipos de transação são:
 * - J: Juros (cobrados na carga inicial em contas negativas)
 * - D: Depósito
 * - S: Saque
 * - P: Pagamento
 * - A: Abertura de nova conta
 * - F: Fechamento de conta
 *
 * A função exibe "OK" na tela se ambos os arquivos forem escritos com sucesso,
 * ou "ERRO" se ocorrer qualquer falha durante o processo de escrita.
 */
int salvar_dados_em_disco(conta *contas, int size, transaction *transacs, int ntransacs)
{
    FILE *file = fopen("contasout.csv", "w");

    if (file == NULL)
    {
        return 0;
    }

    fprintf(file, "%d, %d\n", size, 4);

    for (int i = 0; i < size; i++)
    {
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

    int n = -2, size = 0, ntransacs = 0;
    conta *vet = calloc(1, sizeof(conta));
    transaction *transacs = calloc(1000, sizeof(transaction));

    while (1)
    {

        scanf("%d", &n);
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        switch (n)
        {

        // Se n for -1, encerra o programa
        case -1:
        {
            printf("EXEC FIM\n");
            free(vet);
            free(transacs);
            vet = NULL;
            transacs = NULL;
            return 0;
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
            abrir_conta(vet, &size, transacs, &ntransacs);
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