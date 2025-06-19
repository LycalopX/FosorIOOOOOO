#include "constants.h"

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
void realizar_transferencia(int posicaoOrigem, int posicaoDestino, double valor, conta* contas, int nro_origem, int nro_destino, transaction* transacs, int* ntransacs) {
    
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