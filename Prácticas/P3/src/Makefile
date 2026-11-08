CXX = g++
CXXDEBUG = -g -Wall
CXXSTD=-std=c++17
CXXFLAGS= -Wno-deprecated-register -O0 $(CXXSTD) $(CXXDEBUG)

EXE=compiler

# archivos de prueba que agregue (ejercicio 14)
PRUEBAS = prueba prueba_valida1 prueba_valida2 prueba_invalida1 prueba_invalida2

CPPOBJ = Grammar ParserLL main Production Symbol
FILES=$(addsuffix .cpp, $(CPPOBJ))
OBJS=$(addsuffix .o, $(CPPOBJ))
LOBJS= lexer
SOBJS= objs
###########################################################
DIST_NAME = P2_Prog2_G2101_p11
TESTER_OBJECT = ../tester/tester.o
TEST_EXES = test_element test_stack_element
########################################################
HEADERS_TO_SUBMIT=*.h
SOURCES_TO_SUBMIT=
SUPPORT_TO_SUBMIT=*.txt Makefile

CLEANLIST= $(addsuffix .o, $(CPPOBJ)) Lexer.cpp

.PHONY: all
all: $(LOBJS) $(SOBJS) wc

objs: $(FILES)
	@echo "Checking.."
	$(CXX) $(CXXFLAGS) -c $(FILES)

wc: $(FILES)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) lexer.o



lexer: lexer.ll
	flex $<
	$(CXX) $(CXXFLAGS) -c Lexer.cpp -o lexer.o

# ejecuta el analizador con todos los archivos de prueba, uno tras otro
# (el || true es para que make no marque error cuando una prueba invalida es rechazada)
.PHONY: pruebas
pruebas: all
	@for p in $(PRUEBAS); do \
		echo "===== $$p ====="; \
		./$(EXE) $$p || true; \
	done

.PHONY: clean
clean: 
		rm -rf $(CLEANLIST) lexer.o $(EXE)

.PHONY: dist
dist:
	@echo "Preparing for submission"
	rm -f $(DIST_NAME).zip
	mkdir -p $(DIST_NAME)
	pandoc --latex-engine=xelatex --template=../../pandoc.tex -o $(DIST_NAME)/memoria.pdf memoria.md
	cp $(HEADERS_TO_SUBMIT) $(SOURCES_TO_SUBMIT) $(SUPPORT_TO_SUBMIT) $(DIST_NAME)/
	zip $(DIST_NAME).zip $(DIST_NAME)/*
	rm -rf $(DIST_NAME)
	@echo "Submission is ready"
