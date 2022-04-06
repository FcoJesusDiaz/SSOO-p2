#include <iostream>
#include <thread>
#include <stdlib.h>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include <fstream>

#define N_HILOS 5

int CountLines(std::string filename);

std::vector<int> numLines;

struct Result{
    std::string previous;
    std::string next;
    int line;
};

class Searcher{
    private:
        int id;
        int begin;
        int end;
        //std::fstream fd;
        std::queue<Result> results;
    
    public:
        Searcher(int id, int begin, int end): id(id), begin(begin), end(end){}
        
        void operator()(){

            std::cout<< "Hilo: " << id << ", inicio: " << begin+1 << "final: " << end+1 << std::endl;

        }
};

int main(int argc, char **argv){

    //contamos el numero de lineas del fichero y guardamos en un vector(numLines) el byte en el que comienza cada linea
    int num_lines=CountLines(argv[1]);
    std::cout << "El fichero tiene "<< num_lines <<" lineas"<< std::endl;

    //declaramos el numero de hilos que usaremos en el programa
    int num_threads=atoi(argv[2]);

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

        Searcher s{i+1,l_begin,l_end};
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
    if(mFile.is_open()) 
	{
        //mFile.seekg(0); //lo utilizaremos en cada hilo para indicar desde donde se empieza a leer
		while(mFile.peek()!=EOF)
		{
            numLines.push_back(mFile.tellg()); //metemos en un vector el byte en el que comienza cada linea
            //std::cout<<mFile.tellg()<<std::endl; //indica el byte donde empieza la linea para poder usar seek
			getline(mFile, line);
            lines++;
            
		}

		mFile.close();
		return lines;
	}
	else
		std::cout<<"No se pudo abrir el archivo\n";

    return -1;

}