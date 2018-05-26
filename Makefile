#MAKEFILE

#Author
AUTHORS_NAME=GustavoAlochioLeonardoSantos

#Compiler
CC=g++

#Compiler flags
FLAGS=-Wall -g

#Libs
MATH_LIB=-lm

#Sources variables
SOURCES= $(wildcard src/*.cpp)

#Executable name
EXECUTABLE_NAME=trab1PL

all: $(SOURCES:.cpp=.o)
	@echo Compiling program
	@$(CC) main.cpp $^ $(FLAGS) $(MATH_LIB) -o $(EXECUTABLE_NAME)
	@echo Done

%.o: %.cpp
	@echo Making object file: $@
	@$(CC) -c $< $(FLAGS) -o $@

run: all
	./$(EXECUTABLE_NAME) ./

zip: clean
	zip -r $(AUTHOR_NAME).zip main.cpp src/ Makefile

rebuild: clean all

clean:
	@echo Cleaning object files
	@rm -f *~ $(SOURCES:.cpp=.o) $(EXECUTABLE_NAME)
	@echo Done
