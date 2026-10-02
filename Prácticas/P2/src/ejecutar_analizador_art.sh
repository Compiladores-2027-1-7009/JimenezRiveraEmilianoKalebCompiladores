#!/bin/bash
# Compila y ejecuta analizador_art.ll
# se usa con  sh ejecutar_analizador_art.sh [archivo]   
entrada=${1:-pruebas/programa.art}
flex++ -o analizador_art.cc analizador_art.ll
g++ analizador_art.cc -o analizador_art
echo "===== entrada: $entrada ====="
cat "$entrada"
echo
echo "===== salida ====="
./analizador_art "$entrada" 
tokens="${entrada%.art}.tokens"
if [ -f "$tokens" ]; then
  echo "===== contenido de $tokens ====="
  cat "$tokens"
fi
