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

SSOOIIGLE: $(DIROBJ)searcher.o $(DIROBJ)thread_searcher.o
	$(CC) -o $(DIREXE)$@ $(DIROBJ)searcher.o $(DIROBJ)thread_searcher.o $(LDLIBS)

manager:  $(DIROBJ)manager.o $(DIROBJ)client.o $(DIROBJ)request.o 
	$(CC) -o $(DIREXE)$@ $(DIROBJ)manager.o $(DIROBJ)client.o $(DIROBJ)request.o $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CFLAGS) $^ -o $@

test_manager:
	./$(DIREXE)manager 3 Libros/ACTITUD-DE-VENDEDOR.txt

test1:
	./$(DIREXE)SSOOIIGLE David 

test2:
	./$(DIREXE)SSOOIIGLE sueña 

test3:
	./$(DIREXE)SSOOIIGLE vender 

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRDEBUG) $(DIRHEA)*~ $(DIRSRC)*~
