#MAKEFILE By: Afonso Salvador de Magalhaes
#With support of Kevin Carvalho de Jesus and João Paulo Moura

NAME_PROGRAM = trab2
CC = gcc
FLAGS = -lm -pedantic -Wall 
C_FILES = $(wildcard *.c)
O_FILES = $(patsubst %.c, %.o, $(C_FILES))

all: $(O_FILES) create_executable

CYAN = "\033[1;36m"
YELLOW = "\033[1;33m"
WHITE = "\033[1;37m"
RED = "\033[1;31m"
RESET = "\033[0m"


main.o: main.c
	@ echo $(CYAN)
	@ echo "Compilando main.c ..."
	@ echo $(RESET)
	@ $(CC) -c $< $(FLAGS)

%.o: %.c %.h
	@ echo $(CYAN)
	@ echo "Compilando $< ..."
	@ echo $(RESET)
	@ $(CC) -c $< $(FLAGS)

create_executable: 
	@ echo $(CYAN)
	@ echo "Criando executavel..."
	@ echo $(RESET)
	@ $(CC) -o $(NAME_PROGRAM) $(O_FILES) $(FLAGS)

run: 
	@ echo $(CYAN)
	@ echo "Executando programa ..."
	@ echo $(RESET)
	@ ./$(NAME_PROGRAM) $(f)

test: clean all run

valgrind: 
	@ echo $(YELLOW)
	@ echo "Rodando valgrind ..."
	@ echo $(RESET)
	@ valgrind ./$(NAME_PROGRAM) $(f)

clean: 
	@ echo $(WHITE)
	@ echo "Limpando arquivos temporarios ..."
	@ echo $(RESET)
	@ rm -rf $(NAME_PROGRAM) $(O_FILES)

clean_test:
	@ echo $(RED)
	@ echo "Removendo arquivos gerais ..."
	@ echo $(RESET)
	@ rm -rf $(NAME_PROGRAM) $(O_FILES) *.txt