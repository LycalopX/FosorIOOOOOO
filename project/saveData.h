#include "constants.h"

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