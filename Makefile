# Nome dell'eseguibile
TARGET = QuantumSim

# Compiler
CC = gcc

# Opzioni di compilazione
CFLAGS = -Wall -lm

# File sorgenti
SRCS = main.c quantum_sim.c circuit.c

# File oggetto generati
OBJS = $(SRCS:.c=.o)

# Regola principale
all: $(TARGET)

# Regola per creare l'eseguibile
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Regola per creare file oggetto
%.o: %.c
	$(CC) -c $< $(CFLAGS)

# Pulizia dei file oggetto e dell'eseguibile
clean:
	rm -f $(OBJS) $(TARGET)

# Esecuzione del programma
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run
