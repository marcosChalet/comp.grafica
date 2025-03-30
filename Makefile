SRC = ./src
OBJ = ./obj
BIN = ./bin
INCLUDE = ./include
APP = ./app
FLAG = -O3

OS := $(shell uname -s)

ifeq ($(OS), Linux)
    LIBS = -lm -lGL -lglut -lGLU
else
    LIBS = -lm -lopengl32 -lglu32 -lfreeglut
endif

all: libsPaint myPaintApp

libsPaint: \
	$(OBJ)/utils.o \
	$(OBJ)/events.o \
	$(OBJ)/objects.o \
	$(OBJ)/database.o \
	$(OBJ)/global_state.o \
	$(OBJ)/transformations.o \
	$(OBJ)/opengl_functions.o \
	$(OBJ)/objects_data_structure.o

myPaintApp: clear_app \
	$(BIN)/paint

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	gcc $(FLAG) -c $< -I $(INCLUDE) -o $@

$(BIN)/%: $(APP)/%.c
	gcc $(FLAG) $< $(OBJ)/*.o -I $(INCLUDE) -lm ${LIBS} -Wall -o $@

run:
	$(BIN)/paint

clear:
	rm -rf $(BIN)/* $(OBJ)/*

clear_app:
	rm -rf $(BIN)/*