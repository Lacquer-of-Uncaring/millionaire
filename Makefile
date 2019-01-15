# A simple Makefile for compiling small SDL projects

# set the compiler
#CC := clang

# set the compiler flags
#CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm

# add header files here
#HDRS :=

# add source files here
#SRCS := hello_window.c

# generate names of object files
#OBJS := $(SRCS:.c=.o)

# name of executable
#EXEC := game

# default recipe
#all: $(EXEC)

# recipe for building the final executable
#$(EXEC): $(OBJS) $(HDRS) Makefile
#	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
#clean:
#	rm -f $(EXEC) $(OBJS)

#.PHONY: all clean
#OBJS specifies which files to compile as part of the project
OBJS = 

#CC specifies which compiler we're using
CC = gcc

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = game.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)