#!/bin/bash
# Compila y ejecuta gato.ll
# se usa ejecutnado sh ejecutar_gato.sh [archivo]
entrada=${1:-pruebas/gato.txt}
flex++ -o gato.cc gato.ll
g++ gato.cc -o gato
echo "===== entrada: $entrada ====="
cat "$entrada"
echo
echo "===== salida ====="
./gato "$entrada" 

