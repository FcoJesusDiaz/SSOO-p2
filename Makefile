DIRSRC := src/
DIRINC := include/

CC := g++

all : main

main: 
	$(CC) -o SSOOIIGLE $(DIRSRC)SSOOIIGLE.cpp -pthread -std=c++11

test1:
	./SSOOIIGLE Libros/prueba.txt David 3

clean : 
	rm SSOOIIGLE
