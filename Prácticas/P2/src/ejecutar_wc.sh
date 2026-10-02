#!/bin/bash
# Compila y ejecuta wc.ll
# se usa ejecutando sh ejecutar_wc.sh [archivo]
entrada=${1:-pruebas/texto.txt}
flex++ -o wc.cc wc.ll
g++ wc.cc -o wc
echo "===== entrada: $entrada ====="
cat "$entrada"
echo
echo "===== salida ====="
./wc "$entrada" 
echo "===== wc real (para comparar) ====="
wc "$entrada"
