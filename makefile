CC=g++
OUTPUT=bunhexdump
FLAGS=-Wall --std=c++11

all: main.cpp
	$(CC) main.cpp $(FLAGS) -o $(OUTPUT)

clean:
	rm $(OUTPUT)
