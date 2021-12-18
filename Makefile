.PHONY: clean all
CC=g++
CFLAGS=-std=c++11
LDFLAGS=-fopenmp
SOURCES=A00.cpp A01.cpp A02.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

all: bin $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o bin/$@

bin:
	mkdir -p bin


clean:
	rm -rf bin

