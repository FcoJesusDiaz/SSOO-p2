DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

CFLAGS := -I$(DIRHEA) -c -Wall -std=c++11
LDLIBS := -lpthread -lrt
CC := g++

all : dirs SSOOIIGLE manager

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRDEBUG)

SSOOIIGLE: $(DIROBJ)SSOOIIGLE.o $(DIROBJ)Searcher.o
	$(CC) -o $(DIREXE)$@ $(DIROBJ)SSOOIIGLE.o $(DIROBJ)Searcher.o $(LDLIBS)

manager:  
	$(DIROBJ)manager.o $(DIROBJ)client.o request.o 
	$(CC) -o $(DIREXE)$@ $(DIROBJ)manager.o $(DIROBJ)client.o $(DIROBJ)client.o $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CFLAGS) $^ -o $@

test_manager:
	./$(DIREXE)manager 3 Libros/ACTITUD-DE-VENDEDOR.txt

test1:
	./$(DIREXE)SSOOIIGLE Libros/prueba.txt David 3

test2:
	./$(DIREXE)SSOOIIGLE Libros/VIVE-TU-SUEÑO.txt sueña 10

test3:
	./$(DIREXE)SSOOIIGLE Libros/ACTITUD-DE-VENDEDOR.txt vender 8

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRDEBUG) $(DIRHEA)*~ $(DIRSRC)*~
