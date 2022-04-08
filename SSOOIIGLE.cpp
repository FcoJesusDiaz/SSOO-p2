#include <iostream>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <condition_variable>

#include "../include/Searcher.h"
#include "../include/colors.h"

int CountLines(std::string filename);
void checkArguments(int argc, char **argv);
bool is_integer(char *str);

std::vector<int> numLines;
std::mutex semaphore;
std::mutex semTurn;
std::condition_variable cv;

int main(int argc, char **argv){

    checkArguments(argc,argv);

    int num_lines=CountLines(argv[1]);
    std::cout << "The file has "<< num_lines <<" lines"<< std::endl;

    //declaramos el numero de hilos que usaremos en el programa
    int num_threads=atoi(argv[3]);

    //vectores en los que guardaremos los hilos creados y los objetos searcher
    std::vector<std::thread> v_hilos;
    std::vector<Searcher> v_objetos;

    //Dividimos las lineas entre el numero de hilos creados
    int task_size = num_lines/num_threads;

    for (int i = 0; i < num_threads; i++)
    {
        //variables en las que pondremos la linea de inicio y de final de cada hilo
        int l_begin= i*task_size;
        int l_end= (l_begin+task_size)-1;

        Searcher s{i+1,l_begin,l_end,argv[1],argv[2]};
        v_hilos.push_back(std::thread(s));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));
    
    return 0;
}

int CountLines(std::string filename){
    int lines;
    std::string line;
    std::ifstream mFile(filename);
    if(!mFile.is_open()) 
	{
        std::cerr<< RED <<"Could not open file: "<< filename << RESET << std::endl;
        exit(EXIT_FAILURE);
	}

	while(mFile.peek()!=EOF)
	{
        numLines.push_back(mFile.tellg()); //metemos en un vector el byte en el que comienza cada linea
		getline(mFile, line);
        lines++;
            
	}

	mFile.close();
	return lines;

}

void checkArguments(int argc, char **argv){
    if (argc!=4)
    {
        std::cerr << RED << "Usage: <SSOIIGLE> <file> <word> <number of threads>" <<  RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!is_integer(argv[3])){
        std::cerr << RED << "The number of threads input must be a positive integer" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    
}

bool is_integer(char *str){
    while (*str)
        if (!isdigit(*str++))
            return false;
    return true;
}
