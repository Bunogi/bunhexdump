CC=g++
OUTPUT=bunhexdump
FLAGS=-Wall

all: main.cpp
	$(CC) main.cpp $(FLAGS) -o $(OUTPUT)

clean:
	rm $(OUTPUT)
