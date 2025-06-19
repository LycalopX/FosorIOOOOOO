#include "constants.h"

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