/*
ESTRUTURA DE ARQUIVO DE ENTRADA:

3, 4
00001113, 12345678945, “Fulano da Silva”, 0.00
00002226, 98765432145, “Fulana da Silva”, 1.00
00003339, 11122233318, “John Doe”, -1.00


*/

#include "constants.h"
#include "readData.h"

// Copiado do E03.c
int main()
{
    printf("EXEC MAIN\n");

    int n, size = 0, ntransf;
    conta *vet = NULL;
    transferencia *transf = NULL;

    while (1)
    {

        scanf("%d", &n);

        if (n == -1)
        {
            break;
        }

        switch (n)
        {
        case 0:
        {
            vet = calloc(1, sizeof(conta));
            transf = calloc(1000, sizeof(transferencia));
            read_data(&vet, &size, &transf, &ntransf);
            if (vet == NULL || size <= 0) {
                printf("ERRO\n");
            } else {
                printf("OK\n");
            }
            break;
        }

        case 1:
        {
            break;
        }

        case 2:
        {
            break;
        }

        case 3:
        {
            break;
        }

        case 4:
        {
            break;
        }

        case 5:
        {
            break;
        }

        case 6:
        {
            break;
        }

        case 7:
        {
            break;
        }

        case 8:
        {

            break;
        }

        case 9:
        {

            break;
        }

        default:
            printf("EXEC INVALIDO\n");
            break;
        }
    }

    printf("EXEC FIM\n");
    free(vet);  // Libera a memória alocada
    vet = NULL; // Evita ponteiro solto

    return 0;
}

/*

0 – Carrega dados do arquivo “contasin.csv” (e atualiza com cobrança de juros em contas negativas)
1 – Abre nova conta corrente
2 – Fecha conta corrente existente
3 – Consulta o saldo de conta corrente (pelo nro. da conta)
4 – Consulta o saldo de conta corrente (pelo nro. do CPF)
5 – Deposita valor em uma conta
6 – Realiza o saque de um valor de uma conta
7 – Realiza um pagamento com o saldo de uma conta
8 – Transfere valor de uma conta para outra conta
9 – Salva em disco a posição atual do dia (saldos em contasout.csv e transações em operaout.csv)
-1 – Encerra a execução do programa.

*/