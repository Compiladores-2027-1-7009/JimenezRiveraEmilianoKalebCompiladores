#!/bin/bash
# Ejecuta todos los ejercicios con sus archivos de prueba
for s in numerar_lineas wc binarios mayusculas gato lineas_ab analizador_art; do
  echo
  echo "################ $s ################"
  sh ejecutar_$s.sh
done
