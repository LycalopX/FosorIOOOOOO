#ifndef CONSTANTS_H
#define CONSTANTS_H

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

#endif // CONSTANTS_H