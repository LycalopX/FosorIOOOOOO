#!/bin/bash
set -e

# Garante que o executável antigo seja removido antes de compilar
rm -f main

# Compila todos os arquivos .c
echo "Compilando o código..."
gcc -std=c99 -Wall -o main *.c

# Se não houver argumentos, executa os casos de teste
if [ $# -eq 0 ]; then
    ./main "$@"
else
    ./main "$@"
fi