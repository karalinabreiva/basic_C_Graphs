CC = gcc
OUTPUT = Graph

MAIN_SRC = ./src
INCLUDES = $(MAIN_SRC)/includes
TARGET = ./target

_DEPS = File.h Input.h Nodes.h
DEPS = $(patsubst %,$(INCLUDES)/%,$(_DEPS))

_OBJ = Graph.o File.o Input.o Nodes.o
OBJ = $(patsubst %,$(TARGET)/%,$(_OBJ))


$(TARGET)/%.o: $(MAIN_SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $<

$(OUTPUT): $(OBJ)
	$(CC) -o $(TARGET)/$@ $^

.PHONY: clean

clean:
	rm -f $(TARGET)/*.o
	rm -f $(TARGET)/Graph
