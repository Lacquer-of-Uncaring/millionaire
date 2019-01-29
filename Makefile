#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm -lSDL2


OBJS = main.c rendering.o logic.o menu-items.o
#CC specifies which compiler we're using
CC = gcc

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = game.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

rendering.o : rendering.c rendering.h game.h logic.h
	gcc -c rendering.c -o rendering.o 

logic.o : logic.c logic.h game.h menu-items.h rendering.h
	gcc -c logic.c -o logic.o

menu-items.o : menu-items.c logic.h game.h rendering.h
	gcc -c menu-items.c -o menu-items.o

clean:
	del rendering.o logic.o menu-items.o