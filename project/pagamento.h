#include "constants.h"

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
void realizar_pagamento(int posicaoConta, double valor, conta *contas, int nro_conta, transaction *transacs, int *ntransacs) {
    contas[posicaoConta].saldo -= valor;

    transacs[*ntransacs].nro_conta = nro_conta;
    transacs[*ntransacs].tipo = 'P';
    transacs[*ntransacs].valor = valor;
    (*ntransacs)++;

    printf("CONTA %08d - PGTO %.2f\n", nro_conta, valor);
};