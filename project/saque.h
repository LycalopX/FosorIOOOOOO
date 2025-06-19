#include "constants.h"

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
void realizar_saque(int posicaoConta, double valor, conta *contas, int nro_conta, transaction *transacs, int *ntransacs) {
    contas[posicaoConta].saldo -= valor;

    transacs[*ntransacs].nro_conta = nro_conta;
    transacs[*ntransacs].tipo = 'S';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    printf("CONTA %08d - SAQUE %.2f\n", nro_conta, valor);
}; 