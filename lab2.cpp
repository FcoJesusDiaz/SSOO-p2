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

#define N_HILOS 5

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

            std::cout <<"HILO:" << id <<"Voy a buscar desde la linea"<< begin << "hasta la linea" << end <<"\n" << std::endl;

        }
};

int main(int argc, char **argv){

    std::vector<std::thread> v_hilos;
    std::vector<Searcher> v_objetos;
    int l_begin;
    int l_end;
    
    for (int i = 0; i < N_HILOS; i++)
    {
        l_begin=0;
        l_end=i+90;
        Searcher s{i+1,l_begin,l_end};
        v_hilos.push_back(std::thread(s));
        
    }

    std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));

    

    return 0;
}