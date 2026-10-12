CXX = g++
LEX = flex++
EXEC = compiler

all: $(EXEC)

# genera el analizador lexico a partir de la especificacion en flex
# (es mas o menos como lo que hice con los .sh en la otra carpeta)
Lexer.cpp: lexer.ll tokens.hpp Lexer.hpp
	$(LEX) lexer.ll

# compila el analizador junto con el programa principal
$(EXEC): Lexer.cpp main.cpp Lexer.hpp tokens.hpp
	$(CXX) Lexer.cpp main.cpp -o $(EXEC)

# ejecuta el analizador con el archivo de prueba
run: $(EXEC)
	./$(EXEC) prueba

# borra los archivos generados
clean:
	rm -f Lexer.cpp $(EXEC)

.PHONY: all run clean
