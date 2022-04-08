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

SSOOIIGLE: $(DIROBJ)SSOOIIGLE.o $(DIROBJ)Searcher.o
	$(CC) -o $(DIREXE)$@ $(DIROBJ)SSOOIIGLE.o $(DIROBJ)Searcher.o $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CFLAGS) $^ -o $@

test:
	./$(DIREXE)SSOOIIGLE Libros/prueba.txt David 3

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRDEBUG) $(DIRHEA)*~ $(DIRSRC)*~
