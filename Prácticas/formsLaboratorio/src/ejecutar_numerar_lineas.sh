#!/bin/bash
# Compila y ejecuta numerar_lineas.ll
# se usa ejecutandoo sh ejecutar_numerar_lineas.sh [archivo]   
entrada=${1:-pruebas/texto.txt}
flex++ -o numerar_lineas.cc numerar_lineas.ll
g++ numerar_lineas.cc -o numerar_lineas
echo "===== entrada: $entrada ====="
cat "$entrada"
echo
echo "===== salida ====="
./numerar_lineas "$entrada" 

