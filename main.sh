#!/bin/bash
set -e

# Garante que o executável antigo seja removido antes de compilar
rm -f main


gcc -std=c99 -Wall -o main BANCO_ACME.c

# Se não houver argumentos, executa os casos de teste
if [ $# -eq 0 ]; then
    ./main 0
else
    ./main "$@"
fi