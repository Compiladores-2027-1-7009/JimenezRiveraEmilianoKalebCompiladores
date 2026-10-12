#!/bin/bash
# Compila y ejecuta mayusculas.ll
# se usa ejecutando ejecutar_mayusculas.sh [archivo]   
entrada=${1:-pruebas/minusculas.txt}
flex++ -o mayusculas.cc mayusculas.ll
g++ mayusculas.cc -o mayusculas
echo "===== entrada: $entrada ====="
cat "$entrada"
echo
echo "===== salida ====="
./mayusculas "$entrada" pruebas/mayusculas_salida.txt
echo "===== contenido de pruebas/mayusculas_salida.txt ====="
cat pruebas/mayusculas_salida.txt
