#ifndef CONSTANTS_H
#define CONSTANTS_H

// Definições de constantes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct contas
{
    int nro_conta;
    long long int cpf;
    char nome[30];
    double saldo;
} conta;

typedef struct transferencias
{
    int nro_conta;
    char tipo;
    double valor;
} transferencia;

#endif // CONSTANTS_H