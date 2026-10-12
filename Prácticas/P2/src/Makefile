CXX = g++
LEX = flex++
EXEC = analyzer
HEADERS = headers/Lexer.hpp headers/Parser.hpp headers/Symbols.hpp

# archivos de prueba que agregue (ejercicio 9)
PRUEBAS = prueba prueba_valida1 prueba_valida2 prueba_invalida1 prueba_invalida2

all: $(EXEC)

# genera el analizador lexico a partir de la especificacion en flex
Lexer.cpp: lexer.ll $(HEADERS)
	$(LEX) lexer.ll

# compila el lexer, el parser y el programa principal en un solo ejecutable
$(EXEC): Lexer.cpp Parser.cpp main.cpp $(HEADERS)
	$(CXX) Lexer.cpp Parser.cpp main.cpp -o $(EXEC)

# ejecuta el analizador con el archivo de prueba de la practica
run: $(EXEC)
	./$(EXEC) prueba

# ejecuta el analizador con todos los archivos de prueba, uno tras otro
# (el || true es para que make no marque error cuando una prueba invalida es rechazada)
pruebas: $(EXEC)
	@for p in $(PRUEBAS); do \
		echo "===== $$p ====="; \
		./$(EXEC) $$p || true; \
	done

# borra los archivos generados
clean:
	rm -f Lexer.cpp $(EXEC)

.PHONY: all run pruebas clean
