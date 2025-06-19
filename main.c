/*
ESTRUTURA DE ARQUIVO DE ENTRADA:

3, 4
00001113, 12345678945, “Fulano da Silva”, 0.00
00002226, 98765432145, “Fulana da Silva”, 1.00
00003339, 11122233318, “John Doe”, -1.00


*/

#include "constants.h"
#include "readData.h"

#include "depositar.h"
#include "saque.h"
#include "pagamento.h"
#include "transferencia.h"
#include "saveData.h"
#include "novaconta.h"

int contaExiste(int nro_conta, conta *vet, int size, char *SOD)
{
    // SOD = Single/Origin/Destination

    if (nro_conta <= 0 || nro_conta % 1 != 0)
    {
        printf("ERROCONTA%s\n", SOD);
        return -1;
    }

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

// Copiado do E03.c
int main(int argc, char *argv[])
{
    printf("EXEC MAIN\n");

    int n = -2, size = 0, ntransacs = 0;
    conta *vet = calloc(1, sizeof(conta));
    transaction *transacs = calloc(1000, sizeof(transaction));

    while (1)
    {

        if (n == -2)
        {
            n = atoi(argv[1]);
        }
        else
        {
            scanf("%d", &n);
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 
        }

        switch (n)
        {

        // Se n for -1, encerra o programa
        case -1:
        {
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
            break;
        }

        // 3 – Consulta o saldo de conta corrente (pelo nro. da conta)
        case 3:
        {
            break;
        }

        // 4 – Consulta o saldo de conta corrente (pelo nro. do CPF)
        case 4:
        {
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
            if (salvar_dados_em_disco(vet, size, transacs, ntransacs)) {
                printf("OK\n");
            } else {
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

/*


-1 – Encerra a execução do programa.

*/