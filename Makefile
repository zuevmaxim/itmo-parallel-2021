.PHONY: clean all
CC=g++
CFLAGS=-std=c++11
LDFLAGS=-fopenmp
SOURCES=A00.cpp A01.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

all: bin $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o bin/$@ $(LDFLAGS)

bin:
	mkdir -p bin


clean:
	rm -rf bin

