CXX	=	g++
CXXFLAGS = -Wall -std=c++20		$(shell pkg-config --cflags sdl3)
LDFLAGS = $(shell pkg-config --libs sdl3)

SRC = src/main.cpp	src/chip8.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = chip8

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o:	%.cpp
	$(CXX)	$(CXXFLAGS)	-c	$<	-o	$@

clean:
	rm	-f	$(OBJ)	$(TARGET)









#build:
#	g++	-std=c++20	./*.cpp	-o	main -lSDL
#run:
#	./main
#clean: 
#	rm	main

