OBJS = main.cpp
CC = g++
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
OBJ_NAME = font_maker

all : $(OBJS)
	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm -f $(OBJ_NAME)
