DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

CFLAGS := -I$(DIRHEA) -c -Wall -std=c++11
LDLIBS := -lpthread -lrt
CC := g++

all : dirs manager manager_debug

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRDEBUG)

manager:  $(DIROBJ)manager.o $(DIROBJ)client.o $(DIROBJ)thread_searcher.o $(DIROBJ)request.o $(DIROBJ)searcher.o 
	$(CC) -o $(DIREXE)$@ $(DIROBJ)manager.o $(DIROBJ)searcher.o $(DIROBJ)client.o $(DIROBJ)request.o $(DIROBJ)thread_searcher.o $(LDLIBS)

manager_debug:  $(DIROBJ)manager.o $(DIROBJ)client.o $(DIROBJ)thread_searcher.o $(DIROBJ)request.o $(DIROBJ)searcher.o 
	$(CC) -g -o $(DIREXE)$@ $(DIROBJ)manager.o $(DIROBJ)searcher.o $(DIROBJ)client.o $(DIROBJ)request.o $(DIROBJ)thread_searcher.o $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CFLAGS) $^ -o $@

test:
	./$(DIREXE)manager 3 Libros/dictionary.txt

test_debug:
	./$(DIREXE)manager_debug 1 Libros/dictionary.txt

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRDEBUG) $(DIRHEA)*~ $(DIRSRC)*~
