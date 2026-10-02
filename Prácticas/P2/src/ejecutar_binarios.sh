#!/bin/bash
# Compila y ejecuta binarios.ll
# se usa usando sh ejecutar_binarios.sh [archivo]   
entrada=${1:-pruebas/binarios.txt}
flex++ -o binarios.cc binarios.ll
g++ binarios.cc -o binarios
echo "===== entrada: $entrada ====="
cat "$entrada"
echo
echo "===== salida ====="
./binarios "$entrada" 

