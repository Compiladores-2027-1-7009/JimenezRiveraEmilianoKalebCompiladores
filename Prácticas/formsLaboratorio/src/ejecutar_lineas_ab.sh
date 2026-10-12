#!/bin/bash
# Compila y ejecuta lineas_ab.ll
# se usa ejecutando sh ejecutar_lineas_ab.sh [archivo]  
entrada=${1:-pruebas/lineas.txt}
flex++ -o lineas_ab.cc lineas_ab.ll
g++ lineas_ab.cc -o lineas_ab
echo "===== entrada: $entrada ====="
cat "$entrada"
echo
echo "===== salida ====="
./lineas_ab "$entrada" 

