DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRDEBUG := debug/

CFLAGS := -I$(DIRHEA) -c -Wall -std=c++11
LDLIBS := -lpthread -lrt
CC := g++

all : dirs SSOOIIGLE

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRDEBUG)

debugging:
	$(CC) -I$(DIRHEA) -g $(DIRSRC)SSOOIIGLE.cpp -o $(DIRDEBUG)SSOOIIGLE $(LDLIBS)

SSOOIIGLE: $(DIROBJ)SSOOIIGLE.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(DIROBJ)SSOOIIGLE.o: $(DIRSRC)SSOOIIGLE.cpp
	$(CC) $(CFLAGS) $^ -o $@

test:
	./$(DIREXE)SSOOIIGLE Libros/prueba.txt David 3

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRDEBUG) $(DIRHEA)*~ $(DIRSRC)*~
