#include "constants.h"

FILE *arq;

void read_data(conta *vet, int *size) {
    arq = fopen("dados.csv", "r");
    if (arq == NULL) {
        return; // Erro ao abrir o arquivo
    }
    
    char linha[100];
    fgets(linha, sizeof(linha), arq); // Lê a primeira linha com o número de contas
    *size = atoi(linha);
    
    vet = (conta *)calloc(*size, sizeof(conta));
    if (vet == NULL) {
        fclose(arq);
        return; // Erro ao alocar memória
    }
    int i=0;
    while (fgets(linha, sizeof(linha), arq)) {
        char *tok = strtok(linha, ",");
        vet[i].nro_conta = atoi(tok);

        tok = strtok(NULL, ",");
        vet[i].cpf = atol(tok);

        tok = strtok(NULL, ",");
        strncpy(vet[i].nome, tok, sizeof(vet[i].nome) - 1);
        vet[i].nome[sizeof(vet[i].nome) - 1] = '\0';
        
        tok = strtok(NULL, ",");
        vet[i].saldo = atof(tok);
        i++;
    }
    
    fclose(arq);
    return; // Sucesso

}
