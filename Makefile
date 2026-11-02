EXEC = main

CC = g++

CFLAGS = -Wall -Iincludes

SRC = src/main.cpp \
      src/merge_join.cpp \
      src/ordenacao_externa.cpp \
      src/carregamento.cpp

OBJ = $(SRC:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(EXEC)